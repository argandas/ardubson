/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONElement::BSONElement(void) : _len(0)
{
  // Fill buffer with 0xFF
}

// Public Methods //////////////////////////////////////////////////////////////

void BSONElement::put(const void * source, int size)
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
