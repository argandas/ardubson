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
    assertEqual("hello", be.getKey());
    assertEqual(192, be.getInt());
    assertFalse(be.isString());
    assertFalse(be.isBool());
    assertFalse(be.isDouble());
    assertTrue(be.isInt());
}

unittest(BSONElement_string_t)
{
    BSONElement be;
    be.Key("hello").Value("world");

    /*
    *   \x02              -> 0x02 = type string
    *   hello\x00         -> field name
    *   \x06\x00\x00\x00  -> string length
    *   world!\x00        -> field value
    */

    assertEqual(17, be.len());
    assertEqual(BSON_TYPE_STRING, be.getType());
    assertEqual("hello", be.getKey());
    assertEqual("world", be.getString());
    assertTrue(be.isString());
    assertFalse(be.isInt());
    assertFalse(be.isBool());
    assertFalse(be.isDouble());
}

unittest(BSONElement_Fill)
{
    // BSON encoded char array
    char bsonData[] = {
        0x02,                          // Data type
        'h', 'e', 'l', 'l', 'o', 0x00, // Field name
        0x06, 0x00, 0x00, 0x00,        // String value length (including null terminator)
        'w', 'o', 'r', 'l', 'd', 0x00, // String value
    };

    BSONElement be;
    be.Fill(bsonData, sizeof(bsonData));

    assertEqual(17, be.len());
    assertEqual(BSON_TYPE_STRING, be.getType());
    assertEqual("hello", be.getKey());
    assertEqual("world", be.getString());
    assertTrue(be.isString());
    assertFalse(be.isInt());
    assertFalse(be.isBool());
    assertFalse(be.isDouble());
}

unittest_main()
