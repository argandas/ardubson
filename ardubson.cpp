/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

BSONObjBuilder::BSONObjBuilder(void)
{
  // Fill buffer with 0xFF
  for (int i = 0; i < 1024; i++) {
    _data[i] = 0xFF;
  }
  _idx = 4; // Leave room for size field (4-bytes)
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

int BSONObjBuilder::append(char *key, char *value)
{
  int start = (int)_idx;
  uint32_t len = 0;
  uint32_t off = 0;
  _data[_idx++] = BSON_TYPE_STRING;             // Add type of value to object
  _idx += insertString(_idx, key);              // Add key to object
  off = _idx;
  _idx += 4;                                    // Leave room for size field (4-bytes)
  len = insertString(_idx, value);              // Add value to object
  _idx += len;
  insertUint32(off, len);                       // Add size of field
  return ((int)_idx-start);
}

int BSONObjBuilder::append(char *key, int32_t value)
{
  int start = (int)_idx;
  _data[_idx++] = BSON_TYPE_INT32;              // Add type of value to object
  _idx += insertString(_idx, key);              // Add key to object
  _idx += insertInt32(_idx, value);             // Add value to object
  return ((int)_idx-start);
}

int BSONObjBuilder::append(char *key, int64_t value)
{
  int start = (int)_idx;
  _data[_idx++] = BSON_TYPE_INT64;              // Add type of value to object
  _idx += insertString(_idx, key);              // Add key to object
  _idx += insertInt64(_idx, value);             // Add value to object
  return ((int)_idx-start);
}

char* BSONObjBuilder::toString(void)
{
  char *data = "{\"hello\":\"world\"}";
  return data;
}

void BSONObjBuilder::obj(void)
{
  _data[_idx++] = BSON_EOO;
  insertUint32(0, _idx);
}

int BSONObjBuilder::len(void)
{
  return (int)_idx;
}

uint32_t BSONObjBuilder::insertUint32(uint32_t offset, uint32_t value)
{
  for (uint8_t i = 0; i < 4; i++) {
    _data[offset+i] = (uint8_t)((value >> (i*8)) & 0xFF);
  }
  return 4;
}

uint32_t BSONObjBuilder::insertInt32(uint32_t offset, int32_t value)
{
  for (uint8_t i = 0; i < 4; i++) {
    _data[offset+i] = (uint8_t)((value >> (i*8)) & 0xFF);
  }
  return 4;
}

uint32_t BSONObjBuilder::insertInt64(uint32_t offset, int64_t value)
{
  for (uint8_t i = 0; i < 8; i++) {
    _data[offset+i] = (uint8_t)((value >> (i*8)) & 0xFF);
  }
  return 8;
}

uint32_t BSONObjBuilder::insertString(uint32_t offset, char *data)
{
  uint32_t len = 0;
  for (;*data != 0x00; data++) {
    _data[offset++] = *data;
    len++;
    if (*(data+1) == 0x00) {
      _data[offset++] = BSON_NULL_BYTE;
      len++;
    }
  }
  return len;
}