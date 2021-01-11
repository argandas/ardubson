#include "Arduino.h"
#include <ardubson.h>
#include "ardubsonStreamParser.h"

// Request Commmands
#define REQ_SET_PARAMETER   0x01
#define REQ_GET_PARAMETER   0x02

// Reply Commmands
#define REP_SET_PARAMETER   0xF1
#define REP_GET_PARAMETER   0xF2

BSONStreamParser streamParser;

void processSerialInput();

void messageHandler(BSONObject * bson_obj);
void parseErrorHandler(int error_code);
