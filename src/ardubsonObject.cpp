/*
 ardubson.h - Library for the BSON (Binary-JSON) format.
 Created by Hugo Arganda (argandas), April 6, 2016.
 Released into the public domain.
 */

#include "ardubson.h"

// Constructor /////////////////////////////////////////////////////////////////

BSONObject::BSONObject(char *data)
{
    uint32_t size = 0;
    memcpy((void *)&size, (void *)data, sizeof(uint32_t));

    if ((size > 0) && (size <= BSON_BUFF_SIZE))
    {
        memcpy((void *)&_objData[0], (void *)data, size);
    }
    else
    {
        // Error
        // Serial.println("Invalid size");
    }
}

// Public Methods //////////////////////////////////////////////////////////////

char *BSONObject::rawData(void)
{
    return (char *)&_objData;
}

int32_t BSONObject::len(void)
{
    int32_t size = 0;
    memcpy((void*)&size, (void*)&_objData[0], sizeof(int32_t));
    return size;
}

BSONElement BSONObject::getField(const char *fieldName)
{
    char *e_data = NULL;
    int e_len = 0;
    BSONElement be;
    bool key_found = false;

    // Skip document size field
    int32_t off = sizeof(uint32_t);

    while ((off + 1) < len())
    {
        if ((uint32_t)BSON_EOO == _objData[off])
        {
            break;
        }
        else
        {
            // Get next element data type
            signed char type = _objData[off];
            e_data = &_objData[off];
            off++;
            e_len = 1;

            // Check data type range
            if ((type > (signed char)BSON_MINKEY) && (type < (signed char)BSON_MAXKEY))
            {
                // Get element key
                char *key = (char *)&_objData[off];

                off += strlen(key) + 1;
                e_len += strlen(key) + 1;

                if (type == (char)BSON_TYPE_STRING)
                {
                    // Get field value length
                    uint32_t string_size = 0;
                    memcpy((void *)&string_size, (void *)&_objData[off], sizeof(uint32_t));

                    // Increment field size length
                    off += sizeof(uint32_t);
                    e_len += sizeof(uint32_t);

                    // Increment field value length
                    off += string_size;
                    e_len += string_size;
                }
                else if (type == (char)BSON_TYPE_INT32)
                {
                    // Get value
                    //int32_t val = *(int32_t *) &_objData[off];
                    off += sizeof(int32_t);
                    e_len += sizeof(int32_t);
                }
                else if (type == (char)BSON_TYPE_INT64)
                {
                    // Get value
                    //int64_t val = *(int64_t *) &_objData[off];
                    off += sizeof(int64_t);
                    e_len += sizeof(int64_t);
                }
                else if (type == (char)BSON_TYPE_BOOLEAN)
                {
                    // Get value
                    //char val = *(char *) &_objData[off];
                    off += sizeof(char);
                    e_len += sizeof(char);
                }
                else if (type == (char)BSON_TYPE_NUMBER)
                {
                    // Get value
                    //float val = doublePacked2Float((byte *) &_objData[off], LSBFIRST);
                    off += 8;
                    e_len += 8;
                }

                if (0 == strcmp(fieldName, key))
                {
                    key_found = true;
                    break;
                }
            }
            else
            {
                // Ignore incoming data
                break;
            }
        }
    }

    if (key_found)
    {
        return be.Fill(e_data, e_len);
    }
    else
    {
        return BSONElement(); //Empty result
    }
}

bool BSONObject::appendJSON(const char *data)
{
    int len = strlen((char *)&_jsonStr);
    if (data != NULL)
    {
        if ((len + strlen(data)) < JSON_MAX_SIZE)
        {
            strcat((char *)&_jsonStr, data);
        }
    }
    return true;
}

char *BSONObject::jsonString(int decimal_places)
{
    /* Clear buffer */
    memset(_jsonStr, 0x00, JSON_MAX_SIZE);

    appendJSON("{");

    char *data = (char *)&_objData;

    bool first = true;
    uint32_t len = *(uint32_t *)data;
    data += sizeof(uint32_t);

    while ((uint32_t)(data - (char *)&_objData) < len)
    {
        // Get next element data type
        signed char type = *data;
        data += sizeof(char);

        // Check data type range
        if ((type > (signed char)BSON_MINKEY) && (type < (signed char)BSON_MAXKEY))
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
                uint32_t sz = *(uint32_t *)data;
                data += sizeof(uint32_t);
                appendJSON("\"");
                appendJSON(data);
                appendJSON("\"");
                data += sz;
                break;
            }
            case BSON_TYPE_INT32:
            {
                int32_t val = *(int32_t *)data;
                data += sizeof(int32_t);
                /* TODO: Fix int32 data type being truncated to int by using itoa() */
                char buff[8];
                itoa(val, buff, 10);
                appendJSON(buff);
                break;
            }
            case BSON_TYPE_INT64:
            {
                int64_t val = *(int64_t *)data;
                data += sizeof(int64_t);
                /* TODO: Fix int64 data type being truncated to int by using itoa() */
                char buff[8];
                itoa(val, buff, 10);
                appendJSON(buff);
                break;
            }
            case BSON_TYPE_NUMBER:
            {
                float val = doublePacked2Float((byte *)data, LSBFIRST);
                data += 8;
                appendJSON(String(val, decimal_places).c_str());
                break;
            }
            case BSON_TYPE_BOOLEAN:
            {
                char val = *(char *)data;
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
    return (char *)&_jsonStr;
}
