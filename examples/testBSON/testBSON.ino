#include <ardubson.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("BSON library test");
}

void loop() {
  BSONObjBuilder bob;
  Serial.println("Append string to Builder");
  Serial.println(bob.append("hello", "world!"));
  Serial.print("Key: ");
  Serial.println((char *)&bob._data[5]);

  Serial.print("Value: ");
  Serial.println((char *)&bob._data[15]);
  
  // put your main code here, to run repeatedly:
  while (1);

}
