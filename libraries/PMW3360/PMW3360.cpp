#include "PMW3360.h"

#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>

#include "registers.h"
#include "srom.h"

PMW3360::PMW3360(int slaveSelect, int cpi) {
    _ncs = slaveSelect;
    _cpi = cpi;
    _initComplete = 0;
}

void PMW3360::setup() {
    pinMode(_ncs, OUTPUT);
    SPI.begin();
    SPI.setDataMode(SPI_MODE3);
    SPI.setBitOrder(MSBFIRST);
    _performStartup();
    delay(1000);
    _initComplete = 9;
}

void PMW3360::_adns_com_begin() { digitalWrite(_ncs, LOW); }

void PMW3360::_adns_com_end() { digitalWrite(_ncs, HIGH); }

void PMW3360::_adns_read_reg(byte reg_addr) {
    _adns_com_begin();

    // send adress of the register, with MSBit = 0 to indicate it's a read
    SPI.transfer(reg_addr & 0x7f);
    delayMicroseconds(35);  // tSRAD
    // read data
    byte data = SPI.transfer(0);

    delayMicroseconds(1);  // tSCLK-_NCS for read operation is 120ns
    _adns_com_end();
    delayMicroseconds(19);  //  tSRW/tSRR (=20us) minus tSCLK-_NCS
}

void PMW3360::_adns_write_reg(byte reg_addr, byte data) {
    _adns_com_begin();

    // send adress of the register, with MSBit = 1 to indicate it's a write
    SPI.transfer(reg_addr | 0x80);
    // sent data
    SPI.transfer(data);

    delayMicroseconds(20);  // tSCLK-_NCS for write operation
    _adns_com_end();
    delayMicroseconds(100);  // tSWW/tSWR (=120us) minus tSCLK-_NCS. Could be
                             // shortened, but is looks like a safe lower bound
}

void PMW3360::_adns_upload_firmware() {
    // send the firmware to the chip, cf p.18 of the datasheet
    Serial.println("Uploading firmware...");

    // Write 0 to Rest_En bit of Config2 register to disable Rest mode.
    _adns_write_reg(Config2, 0x00);

    // write 0x1d in SROM_enable reg for initializing
    _adns_write_reg(SROM_Enable, 0x1d);

    // wait for more than one frame period
    delay(10);  // assume that the frame rate is as low as 100fps... even if it
                // should never be that low

    // write 0x18 to SROM_enable to start SROM download
    _adns_write_reg(SROM_Enable, 0x18);

    // write the SROM file (=firmware data)
    _adns_com_begin();
    SPI.transfer(SROM_Load_Burst | 0x80);  // write burst destination adress
    delayMicroseconds(15);

    // send all bytes of the firmware
    unsigned char c;
    for (int i = 0; i < firmware_length; i++) {
        c = (unsigned char)pgm_read_byte(firmware_data + i);
        SPI.transfer(c);
        delayMicroseconds(15);
    }

    // Read the SROM_ID register to verify the ID before any other register
    // reads or writes.
    _adns_read_reg(SROM_ID);

    // Write 0x00 to Config2 register for wired mouse or 0x20 for wireless mouse
    // design.
    _adns_write_reg(Config2, 0x00);

    _adns_com_end();
}

void PMW3360::setCPI(int cpi) {
    int cpival = constrain((cpi / 100) - 1, 0, 0x77);  // limits to 0--119

    _adns_com_begin();
    _adns_write_reg(Config1, cpival);
    _adns_com_end();

    Serial.print("Set cpi to ");
    Serial.println(cpi);
}

void PMW3360::_performStartup() {
    // hard reset
    _adns_com_end();    // ensure that the serial port is reset
    _adns_com_begin();  // ensure that the serial port is reset
    _adns_com_end();    // ensure that the serial port is reset

    _adns_write_reg(Shutdown, 0xb6);  // Shutdown first
    delay(300);

    _adns_com_begin();  // drop and raise _ncs to reset spi port
    delayMicroseconds(40);
    _adns_com_end();
    delayMicroseconds(40);

    _adns_write_reg(Power_Up_Reset, 0x5a);  // force reset
    delay(50);                              // wait for it to reboot

    // read registers 0x02 to 0x06 (and discard the data)
    _adns_read_reg(Motion);
    _adns_read_reg(Delta_X_L);
    _adns_read_reg(Delta_X_H);
    _adns_read_reg(Delta_Y_L);
    _adns_read_reg(Delta_Y_H);
    // upload the firmware
    _adns_upload_firmware();
    delay(10);

    setCPI(_cpi);
    Serial.println("Optical Chip Initialized");
}

void PMW3360::loop(std::function<void(int, int)> callback) {
    byte burstBuffer[12];

    _adns_com_begin();
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE3));

    SPI.transfer(Motion_Burst);
    delayMicroseconds(35);  // waits for tSRAD

    SPI.transfer(burstBuffer, 12);  // read burst buffer
    delayMicroseconds(1);           // tSCLK-_NCS for read operation is 120ns

    SPI.endTransaction();
    /*
    BYTE[00] = Motion    = if the 7th bit is 1, a motion is detected.
    BYTE[01] = Observation
    BYTE[02] = Delta_X_L = dx (LSB)
    BYTE[03] = Delta_X_H = dx (MSB)
    BYTE[04] = Delta_Y_L = dy (LSB)
    BYTE[05] = Delta_Y_H = dy (MSB)
    BYTE[06] = SQUAL     = Surface Quality register, max 0x80
                         - Number of features on the surface = SQUAL * 8
    BYTE[07] = Raw_Data_Sum   = It reports the upper byte of an 18‐bit counter
    which sums all 1296 raw data in the current frame;
                               * Avg value = Raw_Data_Sum * 1024 / 1296
    BYTE[08] = Maximum_Raw_Data  = Max raw data value in current frame, max=127
    BYTE[09] = Minimum_Raw_Data  = Min raw data value in current frame, max=127
    BYTE[10] = Shutter_Upper     = Shutter LSB
    BYTE[11] = Shutter_Lower     = Shutter MSB, Shutter = shutter is adjusted to
    keep the average raw data values within normal operating ranges
    */

    int motion = (burstBuffer[0] & 0x80) > 0;
    int xl = burstBuffer[2];
    int xh = burstBuffer[3];
    int yl = burstBuffer[4];
    int yh = burstBuffer[5];

    int16_t x = xh << 8 | xl;
    int16_t y = yh << 8 | yl;

    _adns_com_end();

    // update only if a movement is detected.
    if (motion) {
        callback(x, y);
    }
}