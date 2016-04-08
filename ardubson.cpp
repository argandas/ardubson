/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "ardubson.h"
#include "ardubsonTypes.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

BSONObjBuilder::BSONObjBuilder(void) : _idx(4)
{
  // Fill buffer with 0xFF
  for (int i = 0; i < 1024; i++) {
    _data[i] = 0x7F;
  }
}

char* BSONObjBuilder::data(void)
{
  return (char*)&_data;
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

BSONObjBuilder& BSONObjBuilder::append(char *key, char *value)
{
  return append(key, value, strlen(value)+1);
}

BSONObjBuilder& BSONObjBuilder::append(char *key, char *value, int size)
{
  appendNum((char) BSON_TYPE_STRING);
  appendStr(key);
  appendNum((int32_t) size);
  appendStr(value);
  return *this;
}

BSONObjBuilder& BSONObjBuilder::append(char *key, bool value) {
  appendNum((char) BSON_TYPE_BOOLEAN);
  appendStr(key);
  appendNum((char)(value ? 1 : 0));
  return *this;
}

BSONObjBuilder& BSONObjBuilder::append(char *key, int value)
{
  return append(key, (int32_t)value);
}

BSONObjBuilder& BSONObjBuilder::append(char *key, int32_t value)
{
  appendNum((char) BSON_TYPE_INT32);
  appendStr(key);
  appendNum(value);
  return *this;
}

BSONObjBuilder& BSONObjBuilder::append(char *key, int64_t value)
{
  appendNum((char) BSON_TYPE_INT64);
  appendStr(key);
  appendNum(value);
}

char* BSONObjBuilder::toString(void)
{
  //char *data = "{\"hello\":\"world\"}";
  char *data;
  uint32_t len = *(uint32_t *)&_data[0];
  uint32_t off = 4;
  while ((off + 1) < len) {
    uint32_t type = *(char *)&_data[off++];
    if (type == (uint32_t)BSON_TYPE_STRING) {
      data = (char *)&_data[off];
      break;
    }
    if (type == (uint32_t)BSON_TYPE_INT32) {
      data = (char *)&_data[off];
      break;
    }
  }
  return data;
}

void BSONObjBuilder::obj(void)
{
  appendNum((char) BSON_EOO);
  *(uint32_t *)&_data = _idx;
}

int BSONObjBuilder::len(void)
{
  return (int)*(uint32_t *)&_data[0];
}

char* BSONObjBuilder::index(void)
{
  return (&_data[0]) + _idx;
}

void BSONObjBuilder::appendNum(char value)
{
  *(char *)index() = value;
  _idx++;
  return;
}

void BSONObjBuilder::appendNum(uint32_t value)
{
  *(uint32_t *)index() = value;
  _idx += 4;
  return;
}

void BSONObjBuilder::appendNum(int32_t value)
{
  *(int32_t *)index() = value;
  _idx += 4;
  return;
}

void BSONObjBuilder::appendNum(int64_t value)
{
  *(int64_t *)index() = value;
  _idx += 8;
  return;
}

void BSONObjBuilder::appendStr(char *data)
{
  for (;*data != BSON_NULL_BYTE; data++) {
    appendNum((char) *data);
  }
  appendNum((char) BSON_NULL_BYTE);
  return;
}