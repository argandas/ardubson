/*
 ardubson.h - Library for the BSON (Binary-JSON) format.
 Created by Hugo Arganda (argandas), April 6, 2016.
 Released into the public domain.
 */

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONObject::BSONObject(char* data)
{
    int size = *(uint32_t *) data;
    if ((size >= 0) && (size <= BSON_BUFF_SIZE))
    {
        memcpy(_objData, data, size);
    }
}

// Public Methods //////////////////////////////////////////////////////////////

char* BSONObject::rawData(void)
{
    return (char *) &_objData;
}

int32_t BSONObject::len(void)
{
    return *(int32_t *) &_objData;
}

BSONElement BSONObject::getField(const char *fieldName)
{
    char* e_data = NULL;
    int e_len = 0;
    BSONElement be;

    uint32_t len = *(uint32_t *) &_objData;
    uint32_t off = sizeof(uint32_t);

    while ((off + 1) < len)
    {
        // Get next element data type
        char type = *(char *) &_objData[off];
        e_data = (char *) &_objData[off];
        off++;
        e_len = 1;


      // Get element key
      char *key = (char *) &_objData[off];
      off += strlen(key) + 1;
      e_len += strlen(key) + 1;
      // Check key
      if (strcmp(fieldName, key) == 0)
      {
          // Save value
          if (type == (char) BSON_TYPE_STRING)
          {
              // Get string size
              uint32_t sz = 0;
              memcpy(&sz, &(_objData[off]), sizeof(uint32_t));
              off += sizeof(uint32_t);
              e_len += sizeof(uint32_t);
              // Get string value
              char *val = (char *) &_objData[off];
              off += sz;
              e_len += sz;
              break;
          }
          else if (type == (char) BSON_TYPE_INT32)
          {
              // Get value
              int32_t val = *(int32_t *) &_objData[off];
              off += sizeof(int32_t);
              e_len += sizeof(int32_t);
              break;
          }
          else if (type == (char) BSON_TYPE_INT64)
          {
              // Get value
              int64_t val = *(int64_t *) &_objData[off];
              off += sizeof(int64_t);
              e_len += sizeof(int64_t);
              break;
          }
          else if (type == (char) BSON_TYPE_BOOLEAN)
          {
              // Get value
              char val = *(char *) &_objData[off];
              off += sizeof(char);
              e_len += sizeof(char);
              break;
          }
          // EOO
          if ((uint32_t) BSON_EOO == _objData[off])
              break;
      }
      else
      {
          // Ignore incoming data
      }

    }
    return be.Fill(e_data, e_len);
}

bool BSONObject::appendJSON(const char* data)
{
    int len = strlen((char *) &_jsonStr);
    if ((len + strlen(data)) < JSON_MAX_SIZE)
    {
        strcat((char *) &_jsonStr, data);
    }
}

char* BSONObject::jsonString(void)
{
    /* Clear buffer */
    memset(_jsonStr, NULL, JSON_MAX_SIZE);

    appendJSON("{");

    char *data = (char *) &_objData;

    bool first = true;
    uint32_t len = *(uint32_t *) data;
    data += sizeof(uint32_t);

    while ((data - (char *) &_objData) < len)
    {
        // Get next element data type
        char type = *data;
        data += sizeof(char);

        // Check data type range
        if ((type > (char) BSON_MINKEY) && (type < (char) BSON_MAXKEY))
        {
            // Add trailing comma
            if (!first)
            {
                appendJSON(",");
            }

            // Get element key
            char *key = data;
            data += strlen(key) + 1;

            appendJSON("\"");
            appendJSON(key);
            appendJSON("\":");

            // Switch according to data type

            switch (type)
            {
            case BSON_TYPE_STRING:
            {
                // Get string size
                uint32_t sz = *(uint32_t *) data;
                data += sizeof(uint32_t);
                appendJSON("\"");
                appendJSON(data);
                appendJSON("\"");
                data += sz;
                break;
            }
            case BSON_TYPE_INT32:
            {
                int32_t val = *(int32_t *) data;
                data += sizeof(int32_t);
                /* TODO: Fix int32 data type being truncated to int by using itoa() */
                char buff[8];
                itoa(val, buff, 10);
                appendJSON(buff);
                break;
            }
            case BSON_TYPE_INT64:
            {
                int64_t val = *(int64_t *) data;
                data += sizeof(int64_t);
                /* TODO: Fix int64 data type being truncated to int by using itoa() */
                char buff[8];
                itoa(val, buff, 10);
                appendJSON(buff);
                break;
            }
            case BSON_TYPE_BOOLEAN:
            {
                char val = *(char *) data;
                data += sizeof(char);
                if (val == 0x1)
                {
                    appendJSON("true");
                }
                else
                {
                    appendJSON("false");
                }
                break;
            }
            default:
                appendJSON("NaN");
            }

            // EOO
            if (BSON_EOO == *data)
                break;
        }
        else
        {
            // Invalid type
            appendJSON("NaT");
            break;
        }
        // At least one element has been added
        first = false;
    }
    appendJSON("}");
    return (char *) &_jsonStr;
}

// Private Methods //////////////////////////////////////////////////////////////
