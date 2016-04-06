/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

BSON::BSON(int a)
{
  _idx = 0;
  // Initialize document size
  _data[_idx++] = 0x16;
  _data[_idx++] = 0x00;
  _data[_idx++] = 0x00;
  _data[_idx++] = 0x00;
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

int BSON::append(char *key, char *value)
{
  char *original;
  int start = (int)_idx;
  int len = 0;
  // Add type of value to object
  _data[_idx++] = BSON_TYPE_STRING;
  // Add key to object
  for (;*key != 0x00; key++) {
    _data[_idx++] = *key;
  }
  _data[_idx++] = BSON_NULL;
  // Add value length to object
  _data[_idx++] = 0x06;
  _data[_idx++] = 0x00;
  _data[_idx++] = 0x00;
  _data[_idx++] = 0x00;
  // Add value to object
  for (;*value != 0x00; value++) {
    _data[_idx++] = *value;
    len++;
  }
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

void BSON::calcDataLen(void)
{
  // Do nothing
}

