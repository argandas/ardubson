/*
  Created
  Aug 29, 2016
  by Hugo Arganda (@argandas)
*/

#include <ardubson.h>

void setup() {
  // Setup serial port
  Serial.begin(9600);
  Serial.println("BSON to JSON example");

  // Create a BSON Builder
  BSONObjBuilder bob;
  BSONElement be;

  be.Key("Hello").Value("world!");

  // Append elements to BSON Builder
  bob.append(be); /* Append BSON Element */
  bob.append("number", (int32_t)3698);
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