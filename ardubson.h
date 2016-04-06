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

#define BSON_TYPE_DOUBLE 0x01 // \x01 double - 64-bit binary floating point
#define BSON_TYPE_STRING 0x02 // \x02 string - UTF-8 string
#define BSON_NULL 0x00 // \x00 NULL terminator

// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class BSON
{
  // user-accessible "public" interface
  public:
    BSON(int);
    uint32_t _idx;
    uint8_t  _data[1024];
    int append(char *key, char *value);
    // int append(String, double);
    // char* data(char*);

  // library-accessible "private" interface
  private:
    void calcDataLen(void);
};

#endif

