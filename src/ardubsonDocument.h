/*
 ardubson.h - Library for the BSON (Binary-JSON) format.
 Created by Hugo Arganda (argandas), April 6, 2016.
 Released into the public domain.
 */
/*
 Check BSON specification at:
 http://bsonspec.org/spec.html
 */

#ifndef ardubson_base_h
#define ardubson_base_h

#include "Arduino.h"
#include "ardubsonTypes.h"
#include "ardubsonConfig.h"
#include "ardubsonElement.h"

class BSONDocument
{
    protected:
        char _doc[BSON_DOC_SIZE];
        uint32_t _idx;
        bool _done;

        char* index(void);
        uint8_t appendNum(char value);
        uint8_t appendBSONElement(BSONElement element);
        uint8_t appendNum(uint32_t value);
        uint8_t appendNum(int32_t value);
        uint8_t appendNum(int64_t value);
        uint8_t appendNum(float value);
        uint8_t appendNum(double value);
        uint8_t appendStr(const char *data);

    public:
        /* Clear builder buffer */
        void reset(void);
};

#endif /* ardubson_base_h */
