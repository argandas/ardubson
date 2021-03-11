/*
 ardubson.h - Library for the BSON (Binary-JSON) format.
 Created by Hugo Arganda (argandas), April 6, 2016.
 Released into the public domain.
 */

#include "ardubsonObjBuilder.h"
#include "ardubsonTypes.h"
#include "ardubsonConfig.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONObjBuilder::BSONObjBuilder(void)
{
    reset();
}

BSONObjBuilder::BSONObjBuilder(char *data, int len)
{
    int i = 0;
    int size = 0;
    reset();
    size = *(uint32_t *)data;
    if ((size == len) && (BSON_DOC_SIZE > len))
    {
        for (i = 0; i < len; i++)
        {
            appendNum(*data++);
        }
        _done = true;
    }
}

// Public Methods //////////////////////////////////////////////////////////////

// Append BSON Element (char*)
BSONObjBuilder &BSONObjBuilder::append(BSONElement element)
{
    appendBSONElement(element);
    return *this;
}

// Append string (char*)
BSONObjBuilder &BSONObjBuilder::append(const char *key, const char *value)
{
    return append(key, value, strlen(value) + 1);
}

// Append string (char*) with a defined length
BSONObjBuilder &BSONObjBuilder::append(const char *key, const char *value, int size)
{
    uint8_t ret = appendNum((char)BSON_TYPE_STRING);
    if (true == ret)
    {
        ret &= appendStr(key);
        if (true == ret)
        {
            ret &= appendNum((int32_t)size);
            if (true == ret)
            {
                ret &= appendStr(value);
                if (true != ret)
                {
                    Serial.println("Failed to append value");
                }
            }
            else
            {
                Serial.println("Failed to append size");
            }
        }
        else
        {
            Serial.println("Failed to append key");
        }
    }
    else
    {
        Serial.println("Failed to append type");
    }
    return *this;
}

// Append boolean
BSONObjBuilder &BSONObjBuilder::append(const char *key, bool value)
{
    uint8_t ret = appendNum((char)BSON_TYPE_BOOLEAN);
    if (true == ret)
    {
        ret &= appendStr(key);
        if (true == ret)
        {
            ret &= appendNum((char)(value ? 1 : 0));
        }
    }
    return *this;
}

// Append int, the value will be saved as int32 type
BSONObjBuilder &BSONObjBuilder::append(const char *key, int16_t value)
{
    return append(key, (int32_t)value);
}

// Append int32
BSONObjBuilder &BSONObjBuilder::append(const char *key, int32_t value)
{
    uint8_t ret = appendNum((char)BSON_TYPE_INT32);
    if (true == ret)
    {
        ret &= appendStr(key);
        if (true == ret)
        {
            ret &= appendNum((int32_t)value);
        }
    }
    return *this;
}

// Append int64
BSONObjBuilder &BSONObjBuilder::append(const char *key, int64_t value)
{
    uint8_t ret = appendNum((char)BSON_TYPE_INT64);
    if (true == ret)
    {
        ret &= appendStr(key);
        if (true == ret)
        {
            ret &= appendNum(value);
        }
    }
    return *this;
}

// Append double
BSONObjBuilder &BSONObjBuilder::append(const char *key, double value)
{
    return BSONObjBuilder::append(key, (float)value);
}
BSONObjBuilder &BSONObjBuilder::append(const char *key, float value)
{
    uint8_t ret = appendNum((char)BSON_TYPE_NUMBER);
    if (true == ret)
    {
        ret &= appendStr(key);
        if (true == ret)
        {
            ret &= appendNum(value);
        }
    }
    return *this;
}

// Generate BSON Object, this finishes the BSON Object builder,
// no more elements should be added after this.
BSONObject BSONObjBuilder::obj(void)
{
    if (!_done)
    {
        appendNum((char)BSON_EOO); // EOO
        *(uint32_t *)&_doc = _idx; // Add frame length
        _done = true;
    }
    return BSONObject((char *)&_doc[0]);
}
