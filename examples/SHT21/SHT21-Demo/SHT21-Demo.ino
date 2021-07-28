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

#include "SHT2X-SOLDERED.h" // include SHT21 library

SHT21 sht; // create SHT21 object

float temp;     // variable to store temperature
float humidity; // variable to store hemidity

void setup()
{
    sht.begin();          // begin Wire(I2C)
    Serial.begin(115200); // begin Serial
}

void loop()
{

    temp = sht.getTemperature();  // get temp from SHT
    humidity = sht.getHumidity(); // get temp from SHT

    Serial.print("Temp: "); // print readings
    Serial.print(temp);
    Serial.print("\t Humidity: ");
    Serial.println(humidity);

    delay(85); // min delay for 14bit temp reading is 85ms
}
