/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/
/*
  Check BSON specification at: 
  http://bsonspec.org/spec.html
*/

#ifndef ardubson_h
#define ardubson_h

#define BSON_EOO        0x00 // \x00 EOO: End-Of-Object
#define BSON_NULL_BYTE  0x00 // \x00 NULL terminator
#define BSON_MINKEY     0xFF // \xFF Min key
#define BSON_MAXKEY     0x7F // \x7F Max key

#define BSON_TYPE_NUMBER        0x01 // \x01 double - 64-bit binary floating point
#define BSON_TYPE_STRING        0x02 // \x02 string - UTF-8 string
#define BSON_TYPE_OBJECT        0x03 // \x03 document - Embedded document
#define BSON_TYPE_ARRAY         0x04 // \x04 document - Array
#define BSON_TYPE_BINARY        0x05 // \x05 binary - Binary data
#define BSON_TYPE_UNDEFINED     0x06 // \x06 Deprecated
#define BSON_TYPE_OID           0x07 // \x07 (byte*12) - ObjectId
#define BSON_TYPE_BOOLEAN       0x08 // \x08
#define BSON_TYPE_DATE          0x09 // \x09
#define BSON_TYPE_NULL          0x0A // \x0A
#define BSON_TYPE_REGEX         0x0B // \x0B
#define BSON_TYPE_REF           0x0C // \x0C
#define BSON_TYPE_CODE          0x0D // \x0D
#define BSON_TYPE_SYMBOL        0x0E // \x0E
#define BSON_TYPE_CODE_W_SCOPE  0x0F // \x0F
#define BSON_TYPE_INT32         0x10 // \x10 int32 - 32-bit integer
#define BSON_TYPE_TIMESTAMP     0x11 // \x11
#define BSON_TYPE_INT64         0x12 // \x12 int64 - 64-bit integer

#include "Arduino.h"

class BSONObjBuilder
{
  public:
    BSONObjBuilder(void);

    uint8_t  _data[1024];

    int append(char *key, char *value);
    int append(char *key, int32_t value);
    int append(char *key, int64_t value);
    char* toString(void);
    void obj(void);
    int len(void);

  private:
    uint32_t insertUint32(uint32_t offset, uint32_t value);
    uint32_t insertInt32(uint32_t offset, int32_t value);
    uint32_t insertInt64(uint32_t offset, int64_t value);
    uint32_t insertString(uint32_t offset, char *data);
    uint32_t _idx;
};

#endif

