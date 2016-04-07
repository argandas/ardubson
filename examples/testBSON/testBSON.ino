#include <ardubson.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("BSON library test");
}

void loop() {
  // Create a BSON Builder
  BSONObjBuilder bob;

  // Append strings to BSON Builder
  bob.append("hello", "world");
  bob.append("year", (int32_t)1986);
  bob.append("seconds", (int64_t) 98765431);

  // Generate BSON Object
  bob.obj();

  /*
    Serial.print("Key: ");
    Serial.println((char *)&bob._data[5]);

    Serial.print("Value: ");
    Serial.println((char *)&bob._data[15]);
  */

  Serial.print("Obj Len: ");
  Serial.println(bob.len());

  Serial.print("String: ");
  Serial.println(bob.toString());

  for (int i = 0; i < bob.len(); i++) {
    Serial.print("0x");
    if (bob._data[i] <= 0xF) Serial.print("0");
    Serial.print(bob._data[i], HEX);
    Serial.print(" ");
    if ((i+1) % 0x8 == 0) Serial.println();
  }
  // put your main code here, to run repeatedly:
  while (1);
}
