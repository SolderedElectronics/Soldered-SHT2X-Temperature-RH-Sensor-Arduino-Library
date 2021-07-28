/**
 **************************************************
 *
 * @file        SHT2X-SOLDERED.cpp
 * @brief       SHT20 and SHT21 SOLDERED sensor class
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Zvonimir Haramustek for soldered.com
 ***************************************************/


#include "SHT2X-SOLDERED.h"


/**
 * @brief                   Initiate I2C communication
 */
void SHT21::begin()
{
    Wire.begin();
}

/**
 * @brief                   Calls humidity measurement with hold master mode.
 *
 * @return float            Returns the humidity in %RH.
 */
float SHT21::getHumidity(void)
{
    uint16_t result; // return variable

    result = readSensor_hm(TRIGGER_RH_MEASUREMENT_NHM);

    return CalcRH(result);
}

/**
 * @brief                   Calls temperature measurement with hold master mode.
 *
 * @return float            Returns the temperature in degrees C.
 */
float SHT21::getTemperature(void)
{
    uint16_t result; // return variable

    result = readSensor_hm(TRIGGER_T_MEASUREMENT_NHM);

    return CalcT(result);
}

/**
 * @brief                   Performs a solfware reset and waits 15ms.
 */
void SHT21::reset()
{
    Wire.beginTransmission(I2C_ADD);
    Wire.write(SOFT_RESET);
    Wire.endTransmission();

    delay(15); // wait for SHT to reset
}

/**
 * @brief                     Returns electronical identification code depending of selected memory location
 *
 * @param  uint8_t return_sn  Memory location
 *
 * @return uint8_t            Identification code.
 */
uint8_t SHT21::getSerialNumber(uint8_t return_sn)
{

    uint8_t serialNumber[8];

    // read memory location 1
    Wire.beginTransmission(I2C_ADD);
    Wire.write(0xFA);
    Wire.write(0x0F);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADD, 8);
    while (Wire.available() < 8)
    {
    }

    serialNumber[5] = Wire.read(); // read SNB_3
    Wire.read();                   // CRC SNB_3 not used
    serialNumber[4] = Wire.read(); // read SNB_2
    Wire.read();                   // CRC SNB_2 not used
    serialNumber[3] = Wire.read(); // read SNB_1
    Wire.read();                   // CRC SNB_1 not used
    serialNumber[2] = Wire.read(); // read SNB_0
    Wire.read();                   // CRC SNB_0 not used

    // read memory location 2
    Wire.beginTransmission(I2C_ADD);
    Wire.write(0xFC);
    Wire.write(0xC9);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADD, 6);
    while (Wire.available() < 6)
    {
    }

    serialNumber[1] = Wire.read(); // read SNC_1
    serialNumber[0] = Wire.read(); // read SNC_0
    Wire.read();                   // CRC SNC_1/SNC_0 not used
    serialNumber[7] = Wire.read(); // read SNA_1
    serialNumber[6] = Wire.read(); // read SNA_0
    Wire.read();                   // CRC SNA_1/SNA_0 not used

    return serialNumber[return_sn];
}

/**
 * @brief                     Reads SHT2X with hold master operation mode.
 *
 * @param  uint8_t command    Temp/hum command
 *
 * @return uint16_t           Temp/hum raw data (16bit scaled)
 */
uint16_t SHT21::readSensor_hm(uint8_t command)
{
    uint8_t checksum;
    uint8_t data[2];
    uint16_t result;
    uint8_t n = 0;
    uint8_t d;

    if (command == TRIGGER_RH_MEASUREMENT_HM || command == TRIGGER_RH_MEASUREMENT_NHM)
        d = 30;
    if (command == TRIGGER_T_MEASUREMENT_HM || command == TRIGGER_T_MEASUREMENT_NHM)
        d = 85;

    Wire.beginTransmission(I2C_ADD);
    Wire.write(command);
    Wire.endTransmission();
    delay(d);
    Wire.requestFrom(I2C_ADD, 3);

    while (Wire.available() < 3)
    {
        delay(10);
        n++;
        if (n > 10)
            return 0;
    }

    data[0] = Wire.read();  // read data (MSB)
    data[1] = Wire.read();  // read data (LSB)
    checksum = Wire.read(); // read checksum

    result = (data[0] << 8);
    result += data[1];

    if (CRC_Checksum(data, 2, checksum))
    {
        reset();
        return 1;
    }

    return result;
}

/**
 * @brief                     Calculates the relative humidity
 *
 * @param  uint16_t rh        Humidity raw value (16bit scaled)
 *
 * @return float              Relative humidity [%RH] (float)
 */
float SHT21::CalcRH(uint16_t rh)
{

    rh &= ~0x0003; // clean last two bits

    return (-6.0 + 125.0 / 65536 * (float)rh); // return relative humidity
}

/**
 * @brief                     Calculates the temperature
 *
 * @param  uint16_t t         Temperature raw value (16bit scaled)
 *
 * @return float              Temperature [°C] (float)
 */
float SHT21::CalcT(uint16_t t)
{

    t &= ~0x0003; // clean last two bits

    return (-46.85 + 175.72 / 65536 * (float)t);
}

/**
 * @brief                       Calculates the temperature
 *
 * @param  uint8_t data[]       Data to test CRC on
 * @param  uint8_t no_of_bytes  Number of bytes in the data array.
 * @param  uint8_t checksum     Expected checksum
 *
 * @return uint8_t              1 if CRC is correct, 0 if not
 */
uint8_t SHT21::CRC_Checksum(uint8_t data[], uint8_t no_of_bytes, uint8_t checksum)
{
    uint8_t crc = 0;
    uint8_t byteCtr;

    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < no_of_bytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }

    return (crc != checksum);
}
