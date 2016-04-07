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
  _idx = 4; // Leave room for size field (4-bytes)
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

int BSONObjBuilder::append(char *key, char *value)
{
  char *original;
  int start = (int)_idx;
  uint32_t len = 0;
  uint32_t off = 0;                             // Offset for string size field

  // ## KEY
  _data[_idx++] = BSON_TYPE_STRING;             // Add type of value to object
  for (;*key != 0x00; key++) {                  // Add key to object
    _data[_idx++] = *key;
    if (*(key+1) == 0x00)
      _data[_idx++] = BSON_NULL;                // Add NULL terminator
  }

  // ## VALUE
  off = _idx;
  len = 0;
  _idx += 4;                                    // Leave room for size field (4-bytes)
  for (;*value != 0x00; value++) {              // Add value to object
    _data[_idx++] = *value;
    len++;
    if (*(value+1) == 0x00) {
      _data[_idx++] = BSON_NULL;                // Add NULL terminator
      len++;
    }
  }
  insertSize(off, len);
  return ((int)_idx-start);
}

/*void BSON::append(String key, double value)
{
  // eventhough this function is public, it can access
  // and modify this library's private variables
  Serial.print("{\"");
  Serial.print(key);
  Serial.print("\":\"");
  Serial.print(value);
  Serial.println("\"}");
}*/

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void BSONObjBuilder::calcDataLen(void)
{
  // Do nothing
}


void BSONObjBuilder::insertSize(uint32_t offset, uint32_t size)
{
  _data[offset] = (uint8_t)(size & 0x000000FF);
  _data[offset++] = (uint8_t)(size >> 8 & 0x000000FF);
  _data[offset++] = (uint8_t)(size >> 16 & 0x000000FF);
  _data[offset++] = (uint8_t)(size >> 24 & 0x000000FF);
}

