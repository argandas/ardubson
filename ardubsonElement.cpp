/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONElement::BSONElement(void) : _len(0)
{
  // Fill buffer with NULL
  for (int i = 0; i < 128; i++)
  {
    _element[i] = BSON_NULL_BYTE;
  }
}

// Public Methods //////////////////////////////////////////////////////////////


BSONElement& BSONElement::Key(const char* key)
{
  // Add NULL type
  put(BSON_NULL_BYTE, sizeof(char));
  // Add key
  put(key, strlen(key)+1);
  return *this;
}

BSONElement& BSONElement::Value(const char* value)
{
  return Value(value, strlen(value) +1);
}

BSONElement& BSONElement::Value(const char* value, int size)
{
  // Add data type
  _element[0] = (char)BSON_TYPE_STRING;
  // Add string size
  put(&size, sizeof(uint32_t));
  // Add string value
  put(value, size);
  return *this;
}

BSONElement& BSONElement::Value(int value)
{
  return Value((int32_t) value);
}

BSONElement& BSONElement::Value(int32_t value)
{
  // Add data type
  _element[0] = (char)BSON_TYPE_INT32;
  // Add value
  put(&value, sizeof(value));
  return *this;
}

BSONElement& BSONElement::Value(int64_t value)
{
  // Add data type
  _element[0] = (char)BSON_TYPE_INT64;
  // Add value
  put(&value, sizeof(value));
  return *this;
}

BSONElement& BSONElement::Value(bool value)
{
  char val = (value ? 1 : 0);
  // Add data type
  _element[0] = (char)BSON_TYPE_BOOLEAN;
  // Add value
  put(&val, sizeof(char));
  return *this;
}

void BSONElement::put(const void* source, int size)
{
  memcpy(_element + _len, source, size);
  _len += size;
}

char* BSONElement::rawData(void)
{
  return (char *)&_element;
}

int BSONElement::len(void)
{
  return _len;
}

char BSONElement::getType(void)
{
  return _element[0];
}

char* BSONElement::getKey(void)
{
  return (char *)&_element[1];
}

bool BSONElement::isString(void)
{
  return (getType() == (char)BSON_TYPE_STRING);
}

bool BSONElement::isInt(void)
{
  return (getType() == (char)BSON_TYPE_INT32);
}

bool BSONElement::isBool(void)
{
  return (getType() == (char)BSON_TYPE_BOOLEAN);
}

char* BSONElement::getString(void)
{
  char* str = "NaS";
  if (isString())
    str = (char *)&_element + sizeof(char) + strlen(getKey()) + 1 + sizeof(uint32_t);
  return str;
}

int BSONElement::getInt(void)
{
  int32_t* val = 0;
  if (isInt())
    val = (int32_t*)((char *)&_element + sizeof(char) + strlen(getKey()) + 1);
  return *val;
}

bool BSONElement::getBool(void)
{
  char* val = 0;
  if (isInt())
    val = (char *)&_element + sizeof(char) + strlen(getKey()) + 1;
  return (*val == 1 ? true : false);
}


// Private Methods //////////////////////////////////////////////////////////////
