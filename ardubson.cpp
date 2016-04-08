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

BSONObjBuilder::BSONObjBuilder(void) : _idx(0)
{
  // Fill buffer with 0xFF
  for (int i = 0; i < 1024; i++) {
    _data[i] = 0xFF;
  }
  _idx += 4; // Leave room for size field (4-bytes)
}

char* BSONObjBuilder::data(void)
{
  return (char*)&_data;
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

BSONObjBuilder& BSONObjBuilder::append(char *key, char *value)
{
  return append(key, value, (uint32_t) strlen(value)+1);
}

BSONObjBuilder& BSONObjBuilder::append(char *key, char *value, uint32_t size)
{
  appendNum((char) BSON_TYPE_STRING);             
  appendStr(key);              
  appendNum(size);
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
  char *data = "{\"hello\":\"world\"}";
  return data;
}

void BSONObjBuilder::obj(void)
{
  _data[_idx++] = BSON_EOO;
  for (char i = 0; i < 4; i++) {
    _data[i] = (char)((_idx >> (i*8)) & 0xFF);
  }
}

int BSONObjBuilder::len(void)
{
  return (int)_idx;
}

void BSONObjBuilder::appendNum(char value)
{
  _data[_idx++] = value;
  return;
}

void BSONObjBuilder::appendNum(uint32_t value)
{
  for (char i = 0; i < 4; i++) {
    appendNum((char)((value >> (i*8)) & 0xFF));
  }
  return;
}

void BSONObjBuilder::appendNum(int32_t value)
{
  char* ptr = (char*)&value;
  for (char i = 0; i < sizeof(value); i++) {
    appendNum(*ptr++);
  }
  return;
}

void BSONObjBuilder::appendNum(int64_t value)
{
  for (char i = 0; i < 8; i++) {
    appendNum((char)((value >> (i*8)) & 0xFF));
  }
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