/*
 ardubson.h - Library for the BSON (Binary-JSON) format.
 Created by Hugo Arganda (argandas), April 6, 2016.
 Released into the public domain.
 */

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONElement::BSONElement(void) :
    _len(1)
{
    memset(e_data, NULL, BSON_ELM_SIZE);
}

BSONElement& BSONElement::Fill(char* data, int len)
{
    _len = 0;
    put(data, len);
    return *this;
}

// Public Methods //////////////////////////////////////////////////////////////


BSONElement& BSONElement::Key(const char* key)
{
    put(key, strlen(key) + 1);
    return *this;
}

void BSONElement::Value(const char* value)
{
    Value(value, strlen(value) + 1);
}

void BSONElement::Value(const char* value, int size)
{
    uint32_t val = (uint32_t) size;
    /* Set data type */
    e_data[0] = BSON_TYPE_STRING;
    /* Add string size */
    put((char*) &val, sizeof(uint32_t));
    /* Add string value */
    put(value, size);
}

void BSONElement::Value(int16_t value)
{
    return Value((int32_t) value);
}

void BSONElement::Value(int32_t value)
{
    /* Set data type */
    e_data[0] = BSON_TYPE_INT32;
    /* Add value */
    put((char *) &value, sizeof(int32_t));
}

void BSONElement::Value(int64_t value)
{
    /* Set data type */
    e_data[0] = BSON_TYPE_INT64;
    /* Add value */
    put((char *) &value, sizeof(int64_t));
}

void BSONElement::Value(bool value)
{
    char val = (value ? 1 : 0);
    /* Set data type */
    e_data[0] = BSON_TYPE_BOOLEAN;
    /* Add value */
    put((char *) &val, sizeof(char));
}

bool BSONElement::put(const char* source, int size)
{
    if ((_len + size) < BSON_ELM_SIZE)
    {
        memcpy(e_data + _len, source, size);
        _len += size;
    }
}

char* BSONElement::rawData(void)
{
    return (char *) &e_data;
}

int BSONElement::len(void)
{
    return _len;
}

char BSONElement::getType(void)
{
    return e_data[0];
}

char* BSONElement::getKey(void)
{
    return &e_data[1];
}

bool BSONElement::isString(void)
{
    return (getType() == (char) BSON_TYPE_STRING);
}

bool BSONElement::isInt(void)
{
    return (getType() == (char) BSON_TYPE_INT32);
}

bool BSONElement::isBool(void)
{
    return (getType() == (char) BSON_TYPE_BOOLEAN);
}

char* BSONElement::getString(void)
{
    char* str = "NaS";
    if (isString())
    {
        str = (char *) &e_data + sizeof(char) + strlen(getKey()) + 1 + sizeof(uint32_t);
    }
    return str;
}

int BSONElement::getInt(void)
{
    int32_t* val = 0;
    // if (isInt()) // TODO need some way to signal an error (avoid NULL pointer)
    {
        val = (int32_t*) ((char *) &e_data + sizeof(char) + strlen(getKey()) + 1);
    }
    return *val;
}

bool BSONElement::getBool(void)
{
    char* val = 0;
    // if (isInt()) // TODO need some way to signal an error (avoid NULL pointer)
    {
        val = (char *) &e_data + sizeof(char) + strlen(getKey()) + 1;
    }
    return (*val == 1 ? true : false);
}

// Private Methods //////////////////////////////////////////////////////////////
