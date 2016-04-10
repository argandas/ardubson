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
    BSONElement(char* data);
    // Methods
    void put(const void * source, int size);
    char* rawData();
    int len();
    // TO-DO: bool exists(void);
    // Create new element
    BSONElement& Key(const char *key);
    BSONElement& Value(const char *value);
    BSONElement& Value(const char *value, int size);
    // TO-DO: BSONElement& Value(bool value);
    BSONElement& Value(int value);
    BSONElement& Value(int32_t value);
    BSONElement& Value(int64_t value);
    BSONElement& Value(bool value);
    // Get attibutes
    char getType(void);
    char* getKey(void);
    // Assertions
    bool isBool(void);
    bool isInt(void);
    bool isString(void);
    // Get values
    char* getString(void);
    int getInt(void);
    bool getBool(void);

  private:
    char _element[128];
    int _len;
};

#endif