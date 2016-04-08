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
    BSONObjBuilder& append(char *key, char *value);
    BSONObjBuilder& append(char *key, char *value, int size);
    BSONObjBuilder& append(char *key, bool value);
    BSONObjBuilder& append(char *key, int value);
    BSONObjBuilder& append(char *key, int32_t value);
    BSONObjBuilder& append(char *key, int64_t value);
    char* toString(void);
    char* data(void);
    void obj(void);
    int len(void);

  private:
    char _data[1024];
    uint32_t _idx;
    char* index(void);
    void appendNum(char value);
    void appendNum(uint32_t value);
    void appendNum(int32_t value);
    void appendNum(int64_t value);
    void appendStr(char *data);
};

#endif

