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
    memcpy((void *)&size, (void *)&_objData[0], sizeof(int32_t));
    return size;
}

BSONElement BSONObject::getField(const char *fieldName)
{
    char *e_data = NULL;
    int e_len = 0;

    BSONElement be;

    getFieldIndex(fieldName, &e_data, &e_len);

    if ((NULL != e_data) && (0 < e_len))
    {
        return be.Fill(e_data, e_len);
    }
    else
    {
#if 0
        Serial.println("key not found!");
        Serial.print("e_data = ");
        Serial.println((int)e_data);
        Serial.print("e_len = ");
        Serial.println(e_len);
#endif
        return BSONElement(); //Empty result
    }
}

void BSONObject::getFieldIndex(const char *fieldName, char **dest, int *size)
{
    char *e_data = NULL;
    int e_len = 0;
    bool key_found = false;

    /*
    Serial.print("fieldName = ");
    Serial.println(fieldName);
    */

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

            /*
            Serial.print("type = ");
            Serial.println(type);
            */

            // Check data type range
            if ((type > (signed char)BSON_MINKEY) && (type < (signed char)BSON_MAXKEY))
            {
                // Get element key
                char *key = (char *)&_objData[off];

                /*
                Serial.print("key = ");
                Serial.println(key);
                */

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
        *dest = e_data;
        *size = e_len;
    }
    else
    {
        // Serial.println("key not found");
        *dest = NULL;
        *size = 0;
    }
}

bool BSONObject::updateField(const char *key, int16_t value)
{
    return updateField(key, (int32_t)value);
}

bool BSONObject::updateField(const char *key, int32_t value)
{
    char *e_data = NULL;
    int e_len = 0;
    bool result = false;

    getFieldIndex(key, &e_data, &e_len);

    if ((NULL != e_data) && (0 < e_len))
    {
        char type = *e_data++;

        if (BSON_TYPE_INT32 == type)
        {
            // Get element key
            char *key = e_data;
            void *val = (void *)key + strlen(key) + 1;

            if (NULL != memcpy(val, (void *)&value, sizeof(int32_t)))
            {
                result = true;
            }
        }
#if 0
        else
        {
            Serial.println("type mismatch!");
            Serial.print("type = ");
            Serial.println(type);
        }
    }
    else
    {
        Serial.println("key not found!");
        Serial.print("e_data = ");
        Serial.println((int)e_data);
        Serial.print("e_len = ");
        Serial.println(e_len);
#endif
    }

    return result;
}

bool BSONObject::updateField(const char *key, int64_t value)
{
    // #helpwanted
    (void)key;
    (void)value;
    return false;
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

    appendJSON("{\r\n    ");

    char *data = (char *)&_objData;

    bool first = true;
    uint32_t len = *(uint32_t *)data;
    data += sizeof(uint32_t);

    while ((uint32_t)(data - (char *)&_objData) < len)
    {
        // Get next element data type
        signed char type = *data;
        data += sizeof(char);

        //Serial.print("type = ");
        //Serial.println(type);

        // Check data type range
        if ((type > (signed char)BSON_MINKEY) && (type < (signed char)BSON_MAXKEY))
        {
            // Add trailing comma
            if (!first)
            {
                appendJSON(",\r\n    ");
            }

            // Get element key
            char *key = data;
            data += strlen(key) + 1;

            //Serial.print("key = ");
            //Serial.println(key);

            appendJSON("\"");
            appendJSON(key);
            appendJSON("\":");

            // Switch according to data type

            switch (type)
            {
            case BSON_TYPE_STRING:
            {
                // Get string size
                uint32_t string_size = 0;
                memcpy((void *)&string_size, (void *)data, sizeof(uint32_t));
                data += sizeof(uint32_t);
                appendJSON("\"");
                appendJSON(data);
                appendJSON("\"");
                data += string_size;
                break;
            }
            case BSON_TYPE_INT32:
            {
                int32_t val = 0;
                memcpy((void *)&val, (void *)data, sizeof(int32_t));
                data += sizeof(int32_t);
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
    appendJSON("\r\n}");
    return (char *)&_jsonStr;
}
