#include <ardubson.h>

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

void setup() {
  Serial.begin(115200);

  //Serial.println("BSON Serial Command Parser Example");

  streamParser.setMessageHandler(messageHandler);
  streamParser.setParseErrorHandler(parseErrorHandler);
}

void loop() {
  processSerialInput();

  // Do other stuff
}

// Calback fired when parsing error is detected
void parseErrorHandler(int error_code) {
  Serial.print("Error parsing message: ");
  Serial.println(error_code);
}

// Calback fired when a complete message has been received
void messageHandler(BSONObject * bson_obj) {

  uint16_t cmd = bson_obj->getField("cmd").getInt();

  if (cmd == REQ_SET_PARAMETER) {

    // Read incoming parameters expected in REQ_SET_PARAMETER command
    String param = bson_obj->getField("param").getString();
    uint16_t param_value = bson_obj->getField("value").getInt();

    // Do something with param and param_value
    // .....
    (void) param;
    (void) param_value;

    // Prepare response message
    BSONObjBuilder bob;  // Create a BSON Builder
    bob.append("cmd", (int32_t)REP_SET_PARAMETER);
    bob.append("status_code", (int32_t)0);
    BSONObject bo = bob.obj();  // Generate BSON Object
    // Send BSON message
    Serial.write((uint8_t *)bo.rawData(), bo.len());
  }

}

// Check for serial port incoming data
void processSerialInput() {
  while (Serial.available()) {
    char c = Serial.read();
    streamParser.analizeIncomingChars(c);
    //Serial.write(c);
  }
  streamParser.update();  // Check for message timeout
}
