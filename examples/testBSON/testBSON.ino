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
  bob.append("year", (int32_t)10);
  bob.append("month", 12);

  // Generate BSON Object
  bob.obj();
  
  Serial.print("Obj Len: ");
  Serial.println(bob.len());

  Serial.print("String: ");
  Serial.println(bob.toString());

  char *ptr = bob.data();
  for (int i = 0; i < bob.len(); i++, ptr++) {
    Serial.print("0x");
    if (*ptr <= 0xF) Serial.print("0");
    Serial.print(*ptr, HEX);
    Serial.print(" ");
    if ((i + 1) % 0x8 == 0) Serial.println();
  }
  // put your main code here, to run repeatedly:
  while (1);
}