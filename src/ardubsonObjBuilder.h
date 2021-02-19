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
#include "ardubsonDocument.h"
#include "ardubsonElement.h"
#include "ardubsonObject.h"

class BSONObjBuilder: public BSONDocument
{
    public:
        /* Void constructor, create an empty BSON Object Builder */
        BSONObjBuilder(void);

        /* Constructor, create an BSON Object Builder from a byte buffer */
        BSONObjBuilder(char *data, int len);

        /* TODO: Append BSON elements */
        BSONObjBuilder& append(BSONElement element);

        /* Append C-string (Must be null terminated ) */
        BSONObjBuilder& append(const char *key, const char *value);

        /* Append Buffer as C-string */
        BSONObjBuilder& append(const char *key, const char *value, int size);

        /* Append bool */
        BSONObjBuilder& append(const char *key, bool value);

        /* Append numbers */
        BSONObjBuilder& append(const char *key, int16_t value);
        BSONObjBuilder& append(const char *key, int32_t value);
        BSONObjBuilder& append(const char *key, int64_t value);
        BSONObjBuilder& append(const char *key, float value);
        BSONObjBuilder& append(const char *key, double value); // In Arduino float==double!!

        /* Create BSON Object */
        BSONObject obj(void);
};

#endif /* ardubson_obj_builder_h */
