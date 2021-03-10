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
        BSONElement(void);
        BSONElement(char *data, int len);
        BSONElement& Fill(char* data, int len);
        // Methods
        char* rawData();
        int len();

        /* Create new element */
        BSONElement& Key(const char *key);
        void Value(const char *value);
        void Value(const char *value, int size);
        void Value(int16_t value);
        void Value(int32_t value);
        void Value(int64_t value);
        void Value(bool value);
        void Value(float value);

        // Get attibutes
        char getType(void);
        char* getKey(void);
        // Assertions
        bool isBool(void);
        bool isInt(void);
        bool isString(void);
        bool isDouble(void);
        // Get values
        char* getString(void);
        int getInt(void);
        bool getBool(void);
        float getDouble(void);

    private:
        bool put(const char* source, int size);
        char* e_type;
        char* e_name;
        char e_data[BSON_ELM_SIZE];
        int _len;
};

#endif
