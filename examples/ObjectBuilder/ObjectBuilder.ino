/*
  Created
  April 8, 2016
  by Hugo Arganda (@argandas)
*/

#include <ardubson.h>

// Create a BSON Builder
BSONObjBuilder bob;

void setup() {
  // Setup serial port
  Serial.begin(9600);
  Serial.println("BSON Object Builder example");

  // Append an element to BSON Builder
  bob.append("hello", "world");

  // Generate BSON Object
  bob.obj();

  // Print object length
  Serial.print("bob Len: ");
  Serial.println(bob.len());

  // Print object data in HEX format
  Serial.print("bob data: ");
  char *ptr = bob.data();
  for (int i = 0; i < bob.len(); i++, ptr++) {
    Serial.print("0x");
    if (*ptr <= 0xF) Serial.print("0");
    Serial.print(*ptr, HEX);
    Serial.print(" ");
    if ((i + 1) % 0x8 == 0) Serial.println();
  }

}

void loop() {
  // Do nothing
}