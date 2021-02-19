#include "ardubsonStreamParser.h"

static errorCallbackPtr parseErrorPointer;
static messageCallbackPtr messagePointer;

BSONStreamParser::BSONStreamParser(void) {
    buffer_index = 0;
}

void BSONStreamParser::update() {
    unsigned long diff = millis() - rx_time_stamp;
    if ((buffer_index > 0) && (diff > COMMAND_TIMEOUT)) {
        //Serial.println("Command timeout!");
        buffer_index = 0;
        if (parseErrorPointer) {
            parseErrorPointer(PARSE_ERROR_TIMEOUT);
        }
    }
}

void BSONStreamParser::setParseErrorHandler(errorCallbackPtr func) {
    parseErrorPointer = func;
}

void BSONStreamParser::setMessageHandler(messageCallbackPtr func) {
    messagePointer = func;
}

void BSONStreamParser::analizeIncomingChars(char c) {
    if (buffer_index < BSON_DOC_SIZE) {
        frame[buffer_index] = c;

        if (buffer_index == 0)
            rx_time_stamp = millis();

        buffer_index++;

        if (buffer_index >= 5) {
            uint32_t size = *(uint32_t *) frame;

            if (size == buffer_index) {
                buffer_index = 0;

                if (size > BSON_DOC_SIZE) {
                    parseErrorPointer(PARSE_ERROR_MESSAGE_SIZE);
                    return;
                }

                if (messagePointer) {
                    BSONObject bo = BSONObject(frame);
                    messagePointer(&bo);
                    return;
                }
            }
        }
    } else {
        //Serial.println("Overflow!");
        buffer_index = 0;
        if (parseErrorPointer) {
            parseErrorPointer(PARSE_ERROR_BUFFER_OVERFLOW);
            return;
        }
    }
}
