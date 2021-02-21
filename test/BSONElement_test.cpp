#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ardubson.h>

unittest(BSONElement_int32_t)
{
    BSONElement be;
    be.Key("hello").Value((int32_t)192);

    assertEqual(10, be.len());
    assertEqual(BSON_TYPE_INT32, be.getType());
    assertEqual(192, be.getInt());
    assertEqual("hello", be.getKey());
    assertTrue(be.isInt());
}

unittest_main()
