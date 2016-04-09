/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONObjBuilder::BSONObjBuilder(void) : _idx(4)
{
  // Fill buffer with 0xFF
  for (int i = 0; i < BSON_BUFF_SIZE; i++) {
    _data[i] = 0x7F;
  }
}

// Public Methods //////////////////////////////////////////////////////////////

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

BSONObject BSONObjBuilder::obj(void)
{
  appendNum((char) BSON_EOO);
  *(uint32_t *)&_data = _idx;
  return BSONObject((char *)&_data);
}

// Private Methods //////////////////////////////////////////////////////////////

char* BSONObjBuilder::index(void)
{
  return (&_data[0]) + _idx;
}

void BSONObjBuilder::appendNum(char value)
{
  *(char *)index() = value;
  _idx += sizeof(char);
  return;
}

void BSONObjBuilder::appendNum(uint32_t value)
{
  *(uint32_t *)index() = value;
  _idx += sizeof(uint32_t);
  return;
}

void BSONObjBuilder::appendNum(int32_t value)
{
  *(int32_t *)index() = value;
  _idx += sizeof(int32_t);
  return;
}

void BSONObjBuilder::appendNum(int64_t value)
{
  *(int64_t *)index() = value;
  _idx += sizeof(int64_t);
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