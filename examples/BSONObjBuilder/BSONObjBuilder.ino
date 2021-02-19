/*
  Created
  April 8, 2016
  by Hugo Arganda (@argandas)
*/

#include <ardubson.h>

void setup() {
  // Setup serial port
  Serial.begin(9600);
  Serial.println("BSON Object Builder example");
  
  // Create a BSON Builder
  BSONObjBuilder bob;
  // Append an element to BSON Builder
  bob.append("hello", "world");
  bob.append("number", (int32_t)1990);
  // Or try inline notation
  bob.append("one", (int32_t)1).append("two", (int32_t)2).append("three", (int32_t)3);

  // Generate BSON Object
  BSONObject bo = bob.obj();
  // Print object length
  Serial.print("\r\nBSON Object data length: ");
  Serial.println(bo.len());
  // Print object data in HEX format
  Serial.println("BSON Object data: ");
  printHex(bo.rawData(), bo.len());

  // Get BSON Object element
  BSONElement be = bo.getField("number");
  Serial.print("\r\nBSON Element data length: ");
  Serial.println(be.len());
  // Print element data in HEX format
  Serial.println("BSON Element data (raw): ");
  printHex(be.rawData(), be.len());
  Serial.print("BSON Element data value: ");
  Serial.println(be.getInt());

  // Get element data inline
  Serial.print("\r\nBSON Element string: ");
  Serial.println(bo.getField("hello").getString());
}

void loop() {
  // Do nothing
}

void printHex(char* data, int len) {
  for (int i = 0; i < len; i++, data++) {
    Serial.print("0x");
    if ((unsigned char)*data <= 0xF) Serial.print("0");
    Serial.print((unsigned char)*data, HEX);
    Serial.print(" ");
    if ((i + 1) % 0x8 == 0) Serial.println();
  }
  Serial.println();
}
