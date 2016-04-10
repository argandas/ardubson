/*
  Created
  April 8, 2016
  by Hugo Arganda (@argandas)
*/

#include <ardubson.h>

// BSON encoded char array
char bsonData[] = {
  0x16, 0x00, 0x00, 0x00,         // Data length
  0x02,                           // Data type
  'h', 'e', 'l', 'l', 'o', 0x00,  // Field name
  0x06, 0x00, 0x00, 0x00,         // String value length (including null terminator)
  'w', 'o', 'r', 'l', 'd', 0x00,  // String value
  0x00                            // EOO
};

void setup() {
  // Setup serial port
  Serial.begin(9600);
  Serial.println("BSON Object example");

  // Create BSON Object
  BSONObject bo(bsonData);
  
  // Print object length
  Serial.print("\r\nBSON Object data length: ");
  Serial.println(bo.len());
  
  // Get BSON Object element
  BSONElement be = bo.getField("hello");
  Serial.print("BSON Element data value: ");
  Serial.println(be.getString());
}

void loop() {
  // Do nothing
  delay(100);
}
