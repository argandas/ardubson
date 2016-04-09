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
  Serial.println("BSON to JSON example");

  // Append elements to BSON Builder
  bob.append("string", "2");
  bob.append("int", 4);
  bob.append("boolean", false);

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