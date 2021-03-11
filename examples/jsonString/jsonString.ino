/*
  Created
  April 8, 2016
  by Hugo Arganda (@argandas)
*/

#include <ardubson.h>

void setup() {
  // Setup serial port
  delay(3000);
  Serial.begin(9600);
  Serial.println("BSON to JSON example");

  // Create a BSON Builder
  BSONObjBuilder bob;

  // Append elements to BSON Builder
  bob.append("string", "2");
  bob.append("int", (int32_t)4);
  bob.append("boolean", (bool)true);

  // Generate BSON Object
  BSONObject bo = bob.obj();

  // Get string object
  char *json = bo.jsonString();

  // Print BSON in JSON format
  Serial.print("JSON: ");
  Serial.println(json);
}

void loop() {
  // Do nothing
}
