/**
 **************************************************
 *
 * @file        SHT2X-SOLDERED.h
 * @brief       SHT2X-SOLDERED.h header file
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Zvonimir Haramustek for soldered.com
 ***************************************************/

#ifndef __SHT2X_SOLDERED__
#define __SHT2X_SOLDERED__

#include "Arduino.h"
#include "Wire.h"

//---------- Defines -----------------------------------------------------------
#define I2C_ADD 0x40 // I2C device address

const uint16_t POLYNOMIAL = 0x131; // P(x)=x^8+x^5+x^4+1 = 100110001

//==============================================================================
#define TRIGGER_T_MEASUREMENT_HM   0XE3 // command trig. temp meas. hold master
#define TRIGGER_RH_MEASUREMENT_HM  0XE5 // command trig. hum. meas. hold master
#define TRIGGER_T_MEASUREMENT_NHM  0XF3 // command trig. temp meas. no hold master
#define TRIGGER_RH_MEASUREMENT_NHM 0XF5 // command trig. hum. meas. no hold master
#define USER_REGISTER_W            0XE6 // command writing user register
#define USER_REGISTER_R            0XE7 // command reading user register
#define SOFT_RESET                 0XFE // command soft reset
//==============================================================================
// HOLD MASTER - SCL line is blocked (controlled by sensor) during measurement
// NO HOLD MASTER - allows other I2C communication tasks while sensor performing
// measurements.


class SHT21
{
  public:
    float getHumidity(void);
    float getTemperature(void);
    void reset();
    uint8_t getSerialNumber(uint8_t return_sn);
    void begin();

  private:
    uint16_t readSensor_hm(uint8_t command);
    float CalcRH(uint16_t rh);
    float CalcT(uint16_t t);
    uint8_t CRC_Checksum(uint8_t data[], uint8_t no_of_bytes, uint8_t checksum);
};

class SHT20 : public SHT21 // Copy of SHT21 class
{
};

#endif
