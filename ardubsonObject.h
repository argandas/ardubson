/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/
/*
  Check BSON specification at:
  http://bsonspec.org/spec.html
*/

#ifndef ardubson_object_h
#define ardubson_object_h

#include "Arduino.h"
#include "ardubsonTypes.h"
#include "ardubsonElement.h"

class BSONObject
{
  public:
    BSONObject(char* data);
    char* rawData(void);
    int len(void);
    BSONElement getField(const char *key);
    char* jsonString(void);

  private:
    char *_objData;
    char _jsonStr[128];
};

#endif