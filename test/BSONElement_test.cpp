#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ardubson.h>

unittest(BSONElement_int32_t)
{
    BSONElement be;
    be.Key("hello").Value((int32_t)192);

    /*
    *   \x10              -> 0x10 = type uint32_t
    *   hello\x00         -> field name
    *   \xC0\x00\x00\x00  -> field value (192)
    */
   
    assertEqual(11, be.len());
    assertEqual(BSON_TYPE_INT32, be.getType());
    assertEqual(192, be.getInt());
    assertEqual("hello", be.getKey());
    assertTrue(be.isInt());
}

unittest_main()
