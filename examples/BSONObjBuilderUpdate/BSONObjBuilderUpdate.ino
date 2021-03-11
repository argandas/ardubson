/*
  Created
  April 8, 2016
  by Hugo Arganda (@argandas)
*/

#include <ardubson.h>

void setup()
{
    // Setup serial port
    Serial.begin(9600);

    delay(3000);

    Serial.println("BSON Object Builder example");

    // Create a BSON Builder
    BSONObjBuilder bob;

    // Append an element to BSON Builder
    bob.append("number", (int32_t)1990);

    // Generate BSON Object
    BSONObject bo = bob.obj();

    Serial.print("\r\nBSON Object data length: ");
    Serial.println(bo.len());

    Serial.println("BSON Object data: ");
    printHex(bo.rawData(), bo.len());

    // Get BSON Object element
    BSONElement be = bo.getField("number");

    Serial.print("BSON Element data value: ");
    Serial.println(be.getInt());
    
    if (bo.updateField("number", (int32_t)300))
    {
        BSONElement be = bo.getField("number");
        Serial.print("BSON Element data new value: ");
        Serial.println(be.getInt());
    }
}

void loop()
{
    // Do nothing
}

void printHex(char *data, int len)
{
    for (int i = 0; i < len; i++, data++)
    {
        Serial.print("0x");
        if ((unsigned char)*data <= 0xF)
            Serial.print("0");
        Serial.print((unsigned char)*data, HEX);
        Serial.print(" ");
        if ((i + 1) % 0x8 == 0)
            Serial.println();
    }
    Serial.println();
}
