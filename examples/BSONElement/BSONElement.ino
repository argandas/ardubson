/*
  Created
  April 9, 2016
  by Hugo Arganda (@argandas)
*/

#include <ardubson.h>

void setup() {
  // Setup serial port
  Serial.begin(9600);
  Serial.println("BSON Element example");
  
  BSONElement be;
  be.Key("hello").Value((int32_t)192);

  Serial.print("\r\nBSON Element data length: ");
  Serial.println(be.len());
  // Print element data in HEX format
  Serial.println("BSON Element data (raw): ");
  printHex(be.rawData(), be.len());
}

void loop() {
  // put your main code here, to run repeatedly:

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
