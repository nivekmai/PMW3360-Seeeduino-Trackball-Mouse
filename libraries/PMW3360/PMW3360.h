#ifndef PMW3360_h
#define PMW3360_h

#include <functional>

#include "Arduino.h"

class PMW3360 {
   public:
    /**
     * Initialize the sensor.
     *
     * @param slaveSelect   This is the SPI "slave select" pin that the sensor
     *                      is hooked up to.
     * @param cpi           The default CPI, can be overridden using `setCPI()`
     */
    PMW3360(int slaveSelect, int cpi);
    /**
     * Setup the sensor. Call this in the Arduino setup method. Expects that
     * Serial has been intialized.
     */
    void setup();
    /**
     * Call in arduino loop method.
     *
     * @param callback      A callback which will be called when the sensor
     *                      detects motion. Will be called with 2 parameters:
     *                      @param X    The X amount of movement since the last
     *                                  loop.
     *                      @param Y    The Y amount of movement since the last
     *                                  loop.
     */
    void loop(std::function<void(int, int)> callback);
    /**
     *  Set the CPI, defaults to 1600 (see configuration.h)
     *  @param cpi          The new CPI value to set in the sensor.
     */
    void setCPI(int cpi);

   private:
    int _ncs;
    int _cpi;
    byte _initComplete;
    void _adns_com_begin();
    void _adns_com_end();
    void _adns_read_reg(byte reg_addr);
    void _adns_write_reg(byte reg_addr, byte data);
    void _adns_upload_firmware();
    void _performStartup();
    void _dispRegisters();
};

#endif