#ifndef ardubson_stream_parser_h
#define ardubson_stream_parser_h

#include "Arduino.h"
#include <ardubson.h>

#define COMMAND_TIMEOUT 500

#define PARSE_ERROR_DATA 0
#define PARSE_ERROR_TIMEOUT 1
#define PARSE_ERROR_BUFFER_OVERFLOW 2
#define PARSE_ERROR_MESSAGE_SIZE 3

typedef void (*errorCallbackPtr)(int error_code);
typedef void (*messageCallbackPtr)(BSONObject * bson_object);

class BSONStreamParser
{
    protected:
        char frame[BSON_DOC_SIZE];
        uint16_t buffer_index;
        unsigned long rx_time_stamp;

    public:
        BSONStreamParser(void);
        void analizeIncomingChars(char c);
        void update();

        void setParseErrorHandler(errorCallbackPtr func);
        void setMessageHandler(messageCallbackPtr func);
        void parseError(void);
        void timeoutError(void);
};

#endif
