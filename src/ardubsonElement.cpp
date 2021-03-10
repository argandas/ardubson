/*
 ardubson.h - Library for the BSON (Binary-JSON) format.
 Created by Hugo Arganda (argandas), April 6, 2016.
 Released into the public domain.
 */

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONElement::BSONElement(void) : _len(0)
{
    memset(e_data, 0x00, BSON_ELM_SIZE);
}

BSONElement::BSONElement(char *data, int len) : _len(0)
{
    memset(e_data, 0x00, BSON_ELM_SIZE);
    Fill(data, len);
}

BSONElement &BSONElement::Fill(char *data, int len)
{
    _len = 0;
    put(data, len);
    return *this;
}

// Public Methods //////////////////////////////////////////////////////////////

BSONElement &BSONElement::Key(const char *key)
{
    put(key, strlen(key) + 1);
    return *this;
}

void BSONElement::Value(const char *value)
{
    Value(value, strlen(value) + 1);
}

void BSONElement::Value(const char *value, int size)
{
    uint32_t val = (uint32_t)size;
    /* Set data type */
    e_data[0] = BSON_TYPE_STRING;
    /* Add string size */
    put((char *)&val, sizeof(uint32_t));
    /* Add string value */
    put(value, size);
}

void BSONElement::Value(int16_t value)
{
    return Value((int32_t)value);
}

void BSONElement::Value(int32_t value)
{
    /* Set data type */
    e_data[0] = BSON_TYPE_INT32;
    /* Add value */
    put((char *)&value, sizeof(int32_t));
}

void BSONElement::Value(int64_t value)
{
    /* Set data type */
    e_data[0] = BSON_TYPE_INT64;
    /* Add value */
    put((char *)&value, sizeof(int64_t));
}

void BSONElement::Value(float value)
{
    /* Set data type */
    e_data[0] = BSON_TYPE_NUMBER;
    /* Add value */
    byte x[8];
    float2DoublePacked(value, x, LSBFIRST);
    put((char *)&x, sizeof(x)); //64 bits
}

void BSONElement::Value(bool value)
{
    char val = (value ? 1 : 0);
    /* Set data type */
    e_data[0] = BSON_TYPE_BOOLEAN;
    /* Add value */
    put((char *)&val, sizeof(char));
}

bool BSONElement::put(const char *source, int size)
{
    bool ret = false;
    if ((_len + size) < BSON_ELM_SIZE)
    {
        memcpy(e_data + _len, source, size);
        _len += size;
        ret = true;
    }
    return ret;
}

char *BSONElement::rawData(void)
{
    return (char *)&e_data;
}

int BSONElement::len(void)
{
    return _len;
}

char BSONElement::getType(void)
{
    return e_data[0];
}

char *BSONElement::getKey(void)
{
    return &e_data[1];
}

bool BSONElement::isString(void)
{
    return (getType() == (char)BSON_TYPE_STRING);
}

bool BSONElement::isInt(void)
{
    return (getType() == (char)BSON_TYPE_INT32);
}

bool BSONElement::isDouble(void)
{
    return (getType() == (char)BSON_TYPE_NUMBER);
}

bool BSONElement::isBool(void)
{
    return (getType() == (char)BSON_TYPE_BOOLEAN);
}

char *BSONElement::getString(void)
{
    char *str = "NaS";
    if (isString())
    {
        str = (char *)&e_data + sizeof(char) + strlen(getKey()) + 1 + sizeof(uint32_t);
    }
    return str;
}

int BSONElement::getInt(void)
{
    int32_t val = 0;
    if (isInt())
    {
        void *src = (void *)&e_data + sizeof(char) + strlen(getKey()) + 1;
        memcpy((void *)&val, src, sizeof(int32_t));
    }
    return val;
}

float BSONElement::getDouble(void)
{
    float val = 0;
    if (isDouble())
    {
        val = doublePacked2Float((byte *)&e_data +
                                     sizeof(char) + strlen(getKey()) + 1,
                                 LSBFIRST);
    }
    return val;
}

bool BSONElement::getBool(void)
{
    bool val = false;
    if (isBool())
    {
        char *src = (char *)&e_data + sizeof(char) + strlen(getKey()) + 1;
        val = *src == 1 ? true : false;
    }
    return val;
}

// Private Methods //////////////////////////////////////////////////////////////
