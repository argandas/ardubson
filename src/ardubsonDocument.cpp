/*
 ardubson.h - Library for the BSON (Binary-JSON) format.
 Created by Hugo Arganda (argandas), April 6, 2016.
 Released into the public domain.
 */

#include "ardubsonDocument.h"
#include "ardubsonTypes.h"
#include "ardubsonConfig.h"
//
#include "IEEE754tools.h"

// Private Methods //////////////////////////////////////////////////////////////

char* BSONDocument::index(void)
{
    return (char *) _doc + _idx;
}

void BSONDocument::reset(void)
{
    memset(&_doc, BSON_NULL_BYTE, BSON_DOC_SIZE);
    _done = false;
    _idx = 4;
    return;
}

uint8_t BSONDocument::appendBSONElement(BSONElement element)
{
    uint8_t ret = false;
    if (BSON_DOC_SIZE > (_idx + element.len()))
    {
        memcpy(index(), element.rawData(), element.len());
        _idx += element.len();
        ret = true;
    }
    return ret;
}


uint8_t BSONDocument::appendNum(char value)
{
    uint8_t ret = false;
    if (BSON_DOC_SIZE > (_idx + sizeof(char)))
    {
        memcpy(index(), (void*)&value, sizeof(char));     
        _idx += sizeof(char);
        ret = true;
    }
    return ret;
}

uint8_t BSONDocument::appendNum(uint32_t value)
{
    uint8_t ret = false;
    if (BSON_DOC_SIZE > (_idx + sizeof(uint32_t)))
    {
        memcpy(index(), (void*)&value, sizeof(uint32_t));     
        _idx += sizeof(uint32_t);
        ret = true;
    }
    return ret;
}

uint8_t BSONDocument::appendNum(int32_t value)
{
    uint8_t ret = false;
    if (BSON_DOC_SIZE > (_idx + sizeof(int32_t)))
    {
        memcpy(index(), (void*)&value, sizeof(int32_t));     
        _idx += sizeof(int32_t);
        ret = true;
    }
    return ret;
}

uint8_t BSONDocument::appendNum(int64_t value)
{
    uint8_t ret = false;
    if (BSON_DOC_SIZE > (_idx + sizeof(int64_t)))
    {
        memcpy(index(), (void*)&value, sizeof(int64_t));     
        _idx += sizeof(int64_t);
        ret = true;
    }
    return ret;
}

uint8_t BSONDocument::appendNum(double value)
{
    return BSONDocument::appendNum((float) value);
}

uint8_t BSONDocument::appendNum(float value)
{
    uint8_t ret = false;
    if (BSON_DOC_SIZE > (_idx + 8))
    {
        byte x[8];
        float2DoublePacked(value, x, LSBFIRST);
        for (int i=0; i<8; i++) {
            /* Append bytes */
            *(char *) index() = (char) x[i];
            _idx += 1;
        }
        ret = true;
    }
    return ret;
}

uint8_t BSONDocument::appendStr(const char *data)
{
    uint8_t ret = false;
    if (BSON_DOC_SIZE > (_idx + strlen(data) + 1))
    {
        ret = true;

        for (; (*data != BSON_NULL_BYTE) && (ret); data++)
        {
            /* Append chars */
            ret &= appendNum((char) *data);
        }

        if (ret)
        {
            /* Add NULL terminator */
            ret &= appendNum((char) BSON_NULL_BYTE);
        }
    }
    return ret;
}
