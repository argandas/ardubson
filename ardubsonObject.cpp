/*
  ardubson.h - Library for the BSON (Binary-JSON) format.
  Created by Hugo Arganda (argandas), April 6, 2016.
  Released into the public domain.
*/

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONObject::BSONObject(char* data)
{
  int size = *(uint32_t *)data;
  if ((size >= 0) && (size <= BSON_BUFF_SIZE))
  {
    memcpy(_objData, data, size);
  }
}

// Public Methods //////////////////////////////////////////////////////////////

char* BSONObject::rawData(void)
{
  return (char *)&_objData;
}

int BSONObject::len(void)
{
  return (int)(*(uint32_t *)&_objData);
}

BSONElement BSONObject::getField(const char *fieldName)
{
  BSONElement be;

  uint32_t len = *(uint32_t *)&_objData;
  uint32_t off = sizeof(uint32_t);

  while ((off + 1) < len) {
    // Get next element data type
    char type = *(char *)&_objData[off++];
    // Check data type range
    if ((type > (char)BSON_MINKEY) && (type < (char)BSON_MAXKEY))
    {
      // Get element key
      char *key = (char *)&_objData[off];
      off += strlen(key) + 1;
      // Check key
      if (strcmp(fieldName, key) == 0)
      {
        // Save field type & name
        be.put(&type, sizeof(type));
        be.put(key, strlen(key) + 1);
        // Save value
        if (type == (char)BSON_TYPE_STRING)
        {
          // Get string size
          uint32_t sz = *(uint32_t *)&_objData[off];
          off += sizeof(sz);
          // Get string value
          char *val = (char *)&_objData[off];
          off += sz;
          // Save string size & value
          be.put(&sz, sizeof(sz));
          be.put(val, strlen(val) + 1);
        }
        else if (type == (uint32_t)BSON_TYPE_INT32)
        {
          // Get value
          int32_t val = *(int32_t *)&_objData[off];
          off += sizeof(val);
          // Save value
          be.put(&val, sizeof(val));
        }
        else if (type == (uint32_t)BSON_TYPE_BOOLEAN)
        {
          // Get value
          char val = *(char *)&_objData[off];
          off += sizeof(val);
          // Save value
          be.put(&val, sizeof(val));
        }
        // EOO
        if ((uint32_t)BSON_EOO == _objData[off]) break;
      }
      else
      {
        // Ignore incoming data
      }
      // break;
    }
    else
    {
      // Invalid type
      break;
    }
  }
  return be;
}

char* BSONObject::jsonString(void)
{
  char *data = (char *)&_jsonStr;
  strcpy(data, "{");

  bool first = true;
  uint32_t len = *(uint32_t *)&_objData;
  uint32_t off = sizeof(uint32_t);

  while ((off + 1) < len) {
    // Get next element data type
    char type = *(char *)&_objData[off++];
    // Check data type range
    if ((type > (char)BSON_MINKEY) && (type < (char)BSON_MAXKEY))
    {
      // Add trailing comma
      if (!first) strcat(data, ", ");
      // Get element key
      char *key = (char *)&_objData[off];
      strcat(data, "\"");
      strcat(data, key);
      strcat(data, "\":");
      off += strlen(key) + 1;
      // Switch according to data type
      if (type == (char)BSON_TYPE_STRING)
      {
        // Get string size
        uint32_t sz = *(uint32_t *)&_objData[off];
        off += sizeof(uint32_t);
        // Get value
        char *val = (char *)&_objData[off];
        strcat(data, "\"");
        strcat(data, val);
        strcat(data, "\"");
        off += sz;
      }
      else if (type == (char)BSON_TYPE_INT32)
      {
        // Get value
        char buff [12];
        int32_t val = *(int32_t *)&_objData[off];
        itoa(val, buff, 10);
        strcat(data, buff);
        off += sizeof(val);
      }
      else if (type == (char)BSON_TYPE_BOOLEAN)
      {
        // Get value
        char val = *(char *)&_objData[off];
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
      if ((uint32_t)BSON_EOO == _objData[off]) break;
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
