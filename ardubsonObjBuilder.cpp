/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONObjBuilder::BSONObjBuilder(void) : _idx(4),  _done(false)
{
  // Fill buffer with 0xFF
  for (int i = 0; i < BSON_BUFF_SIZE; i++)
  {
    _data[i] = BSON_NULL_BYTE;
  }
}

BSONObjBuilder::BSONObjBuilder(char *data, int len) : _done(true)
{
  int size = *(uint32_t *)data;
  // Check incoming size vs len
  if (size == len)
  {
    memcpy(_data, data, size);
  }
}

// Public Methods //////////////////////////////////////////////////////////////

// Append string
BSONObjBuilder& BSONObjBuilder::append(const char *key, char *value)
{
  return append(key, value, strlen(value) + 1);
}


// Append string with a defined len
BSONObjBuilder& BSONObjBuilder::append(const char *key, char *value, int size)
{
  appendNum((char) BSON_TYPE_STRING);
  appendStr(key);
  appendNum((int32_t) size);
  appendStr(value);
  return *this;
}

// Append boolean
BSONObjBuilder& BSONObjBuilder::append(const char *key, bool value) {
  appendNum((char) BSON_TYPE_BOOLEAN);
  appendStr(key);
  appendNum((char)(value ? 1 : 0));
  return *this;
}

// Append int, the value will be saved as int32 type
BSONObjBuilder& BSONObjBuilder::append(const char *key, int value)
{
  return append(key, (int32_t)value);
}

// Append int32
BSONObjBuilder& BSONObjBuilder::append(const char *key, int32_t value)
{
  appendNum((char) BSON_TYPE_INT32);
  appendStr(key);
  appendNum(value);
  return *this;
}

// Append int64
BSONObjBuilder& BSONObjBuilder::append(const char *key, int64_t value)
{
  appendNum((char) BSON_TYPE_INT64);
  appendStr(key);
  appendNum(value);
}

// Generate BSON Object, this finishes the BSON Object builder,
// no more elements should be added after this.
BSONObject BSONObjBuilder::obj(void)
{
  if (!_done) 
  {
    appendNum((char) BSON_EOO);  // EOO
    *(uint32_t *)&_data = _idx;  // Add frame length
    _done = true;
  }
  return BSONObject((char *)_data);
}

// Private Methods //////////////////////////////////////////////////////////////

char* BSONObjBuilder::index(void)
{
  return (char *)_data + _idx;
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

void BSONObjBuilder::appendStr(const char *data)
{
  for (; *data != BSON_NULL_BYTE; data++) {
    appendNum((char) *data);
  }
  appendNum((char) BSON_NULL_BYTE);
  return;
}