/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONObjBuilder::BSONObjBuilder(void) : _idx(4),  _done(false)
{
  // Fill buffer with 0x00
  memset(&_data, BSON_NULL_BYTE, BSON_BUFF_SIZE);
}

BSONObjBuilder::BSONObjBuilder(char *data, int len) : _done(true)
{
  int size = *(uint32_t *)data;
  // Check incoming size vs len
  if ((size == len) && (BSON_BUFF_SIZE > len))
  {
    memcpy(_data, data, size);
  }
}

// Public Methods //////////////////////////////////////////////////////////////

// Append string (char*)
BSONObjBuilder& BSONObjBuilder::append(const char *key, char *value)
{
  return append(key, value, strlen(value) + 1);
}

// Append string (char*) with a defined length
BSONObjBuilder& BSONObjBuilder::append(const char *key, char *value, int size)
{
  uint8_t ret = appendNum((char) BSON_TYPE_STRING);
  if (true == ret)
  {
    ret &= appendStr(key);
    if (true == ret)
    {
      ret &= appendNum((int32_t) size);
      if (true == ret)
      {
        ret &= appendStr(value);
      }
    }
  }
  return *this;
}

// Append boolean
BSONObjBuilder& BSONObjBuilder::append(const char *key, bool value) {
  uint8_t ret = appendNum((char) BSON_TYPE_BOOLEAN);
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
BSONObjBuilder& BSONObjBuilder::append(const char *key, int value)
{
  return append(key, (int32_t)value);
}

// Append int32
BSONObjBuilder& BSONObjBuilder::append(const char *key, int32_t value)
{
  uint8_t ret = appendNum((char) BSON_TYPE_INT32);
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

// Append int64
BSONObjBuilder& BSONObjBuilder::append(const char *key, int64_t value)
{
  uint8_t ret = appendNum((char) BSON_TYPE_INT64);
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
    appendNum((char) BSON_EOO);  // EOO
    *(uint32_t *)&_data = _idx;  // Add frame length
    _done = true;
  }
  return BSONObject((char *)_data);
}

// Reset (clean) BSON Object Builder buffer
void BSONObjBuilder::reset(void)
{
  // Fill buffer with 0x00
  memset(&_data, BSON_NULL_BYTE, BSON_BUFF_SIZE);
  _done = false;
  _idx = 4;
  return;
}

// Private Methods //////////////////////////////////////////////////////////////

char* BSONObjBuilder::index(void)
{
  return (char *)_data + _idx;
}

uint8_t BSONObjBuilder::appendNum(char value)
{
  uint8_t ret = false;
  if(BSON_BUFF_SIZE > (_idx + sizeof(char)))
  {
    *(char *)index() = value;
    _idx += sizeof(char);
    ret = true;
  }
  return ret;
}

uint8_t BSONObjBuilder::appendNum(uint32_t value)
{
  uint8_t ret = false;
  if(BSON_BUFF_SIZE > (_idx + sizeof(uint32_t)))
  {
    *(uint32_t *)index() = value;
    _idx += sizeof(uint32_t);
    ret = true;
  }
  return ret;
}

uint8_t BSONObjBuilder::appendNum(int32_t value)
{
  uint8_t ret = false;
  if(BSON_BUFF_SIZE > (_idx + sizeof(int32_t)))
  {
    *(int32_t *)index() = value;
    _idx += sizeof(int32_t);
    ret = true;
  }
  return ret;
}

uint8_t BSONObjBuilder::appendNum(int64_t value)
{
  uint8_t ret = false;
  if(BSON_BUFF_SIZE > (_idx + sizeof(int64_t)))
  {
    *(int64_t *)index() = value;
    _idx += sizeof(int64_t);
    ret = true;
  }
  return ret;
}

uint8_t BSONObjBuilder::appendStr(const char *data)
{
  uint8_t ret = false;
  if(BSON_BUFF_SIZE > (_idx + strlen(data) + 1))
  {
    ret = true;
    for (; (*data != BSON_NULL_BYTE) && (true == ret); data++) {
      ret &= appendNum((char) *data);
    }
    if (true == ret)
    {
      ret &= appendNum((char) BSON_NULL_BYTE);
    }
  }
  return ret;
}