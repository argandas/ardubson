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

#include "Arduino.h"
#include "ardubsonTypes.h"

class BSONObjBuilder
{
  public:
    BSONObjBuilder(void);

    uint8_t  _data[1024];

    BSONObjBuilder& append(char *key, char *value);
    BSONObjBuilder& append(char *key, int32_t value);
    BSONObjBuilder& append(char *key, int64_t value);
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

