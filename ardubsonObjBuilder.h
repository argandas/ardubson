/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/
/*
  Check BSON specification at:
  http://bsonspec.org/spec.html
*/

#ifndef ardubson_obj_builder_h
#define ardubson_obj_builder_h

#include "Arduino.h"
#include "ardubsonTypes.h"
#include "ardubsonElement.h"
#include "ardubsonObject.h"

class BSONObjBuilder
{
  public:
    BSONObjBuilder(void);
    BSONObjBuilder(char *data, int len);
    // Append BSON elements
    // TO-DO: BSONObjBuilder& append(BSONElement element);
    BSONObjBuilder& append(const char *key, char *value);
    BSONObjBuilder& append(const char *key, char *value, int size);
    BSONObjBuilder& append(const char *key, bool value);
    BSONObjBuilder& append(const char *key, int value);
    BSONObjBuilder& append(const char *key, int32_t value);
    BSONObjBuilder& append(const char *key, int64_t value);
    // Create BSON Object
    BSONObject obj(void);

  private:
    char _data[BSON_BUFF_SIZE];
    uint32_t _idx;
    bool _done;
    char* index(void);
    void appendNum(char value);
    void appendNum(uint32_t value);
    void appendNum(int32_t value);
    void appendNum(int64_t value);
    void appendStr(const char *data);
};

#endif