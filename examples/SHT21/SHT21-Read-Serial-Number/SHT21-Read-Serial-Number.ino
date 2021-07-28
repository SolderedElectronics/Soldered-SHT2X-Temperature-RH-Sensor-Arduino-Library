/**
 **************************************************
 *
 * @file        SHT21-Read-Serial-Number.ino
 * @brief       Prints hardcored SHT21 Serial Number to Serial monitor.
 *
 *  Learn more about the board at https://solde.red/[_sku]
 *
 * @authors     Zvonimir Haramustek for soldered.com
 ***************************************************/


#include "SHT2X-SOLDERED.h"

SHT21 sht;

void setup()
{
    Wire.begin();         // begin Wire
    Serial.begin(115200); // begin Serial

    // call following as argument in getSerialNumber function to print SN
    //    0 - for SNC_0
    //    1 - for SNC_1
    //    2 - for SNB_0
    //    3 - for SNB_1
    //    4 - for SNB_2
    //    5 - for SNB_3
    //    6 - for SNA_0
    //    7 - for SNA_1

    // printing all serial numbers to Serial monitor
    for (int i = 0; i < 8; i++)
    {
        Serial.print(sht.getSerialNumber(i), BIN);
    }
    Serial.println();
}

void loop()
{
}
