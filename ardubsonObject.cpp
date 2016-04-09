/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONObject::BSONObject(char* data)
{
  // Fill buffer with 0xFF
  for (int i = 0; i < BSON_BUFF_SIZE; i++) {
    _data[i] = 0x7F;
  }
  *(uint32_t *)&_data = 5;
  // Fill buffer with 0xFF
  //for (int i = 0; i < BSON_BUFF_SIZE; i++, data++) {
  //  _data[i] = *data;
  //}
}

// Public Methods //////////////////////////////////////////////////////////////

char* BSONObject::rawData(void)
{
  return (char*)&_data;
}


int BSONObject::len(void)
{
  return (int)*(uint32_t *)&_data;
}


char* BSONObject::jsonString(void)
{
  char *data = (char *)&_jsonStr;
  strcpy(data, "{");

  bool first = true;
  uint32_t len = *(uint32_t *)&_data;
  uint32_t off = sizeof(uint32_t);

  while ((off + 1) < len) {
    // Get next element data type
    char type = *(char *)&_data[off++];
    // Check data type range
    if ((type > (char)BSON_MINKEY) && (type < (char)BSON_MAXKEY)) 
    {
      // Add trailing comma
      if (!first) strcat(data, ", ");
      // Get element key
      char *key = (char *)&_data[off];
      strcat(data, "\"");
      strcat(data, key);
      strcat(data, "\":");
      off += strlen(key)+1;
      // Switch according to data type
      if (type == (char)BSON_TYPE_STRING) 
      {
        // Get string size
        uint32_t sz = *(uint32_t *)&_data[off];
        off += sizeof(uint32_t);
        // Get value
        char *val = (char *)&_data[off];
        strcat(data, "\"");
        strcat(data, val);
        strcat(data, "\"");
        off += sz;
      } 
      else if (type == (uint32_t)BSON_TYPE_INT32) 
      {
        // Get value
        char buff [12];
        int32_t val = *(int32_t *)&_data[off];
        itoa(val, buff, 10);
        strcat(data, buff);
        off += sizeof(val);
      }  
      else if (type == (uint32_t)BSON_TYPE_BOOLEAN) 
      {
        // Get value
        char val = *(char *)&_data[off];
        if (val == 0x1) strcat(data, "true");
        else strcat(data, "false");
        off += sizeof(val);
      } 
      else
      {
        strcat(data, "<Unexpected type>");
        // Unexpected type
        break;
      }
      // EOO
      if ((uint32_t)BSON_EOO == _data[off]) break;
    }
    else
    {
      // Invalid type
      strcat(data, "<Invalid type>");
      break;
    }   
    // At least one element has been added
    first = false;   
  }
  strcat(data, "}");
  return data;
}

// Private Methods //////////////////////////////////////////////////////////////
