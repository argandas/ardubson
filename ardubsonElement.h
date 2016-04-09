/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/
/*
  Check BSON specification at: 
  http://bsonspec.org/spec.html
*/

#ifndef ardubson_element_h
#define ardubson_element_h

#include "Arduino.h"
#include "ardubsonTypes.h"

class BSONElement
{
  public:
    BSONElement(void);
    BSONElement& Key(char *key);
    BSONElement& Value(char *value);
    BSONElement& Value(char *value, int size);
    BSONElement& Value(bool value);
    BSONElement& Value(int value);
    BSONElement& Value(int32_t value);
    BSONElement& Value(int64_t value);
    // Methods
    char getType(void);
    char* getKey(char *buff);
    char* getString(void);
    int getInt(void);
    bool getBool(void);
    char* rawData();
    int size();
    bool exists(void);
    // Assertions
    bool isBool(void);
    bool isInt(void);
    bool isString(void);

  private:
    char _type;
    char _data[32];
    char _key[32];
    int _size;
};

#endif