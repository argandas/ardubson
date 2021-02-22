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
#include "ardubsonConfig.h"

class BSONElement
{
public:
    /* Constructors */
    BSONElement(void);
    BSONElement(char *data, int len);
    
    /* Create new element */
    BSONElement &Key(const char *key);
    void Value(const char *value);
    void Value(const char *value, int size);
    void Value(int16_t value);
    void Value(int32_t value);
    void Value(int64_t value);
    void Value(bool value);
    void Value(float value);
    void Value(double value);

    // Get methods
    char *rawData();
    int len();
    char getType(void);
    char *getKey(void);

    // Assertions
    bool isBool(void);
    bool isInt(void);
    bool isString(void);
    bool isDouble(void);
    
    // Get values
    char *getString(void);
    int getInt(void);
    bool getBool(void);
    float getDouble(void);

private:
    void e_name(const char e_name);
    bool put(const char *source, int size);
    char *e_type;
    char e_data[BSON_ELM_SIZE];
    int _len;
};

#endif
