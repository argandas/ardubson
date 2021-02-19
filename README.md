[![Arduino CI](https://github.com/argandas/ardubson/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/argandas/ardubson/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/v/release/argandas/ardubson)](https://github.com/argandas/ardubson/releases)

# ardubson
Lightweight BSON (Binary JSON) Library for Arduino

## Getting started

Please check the examples folder for detailed explanations

## Supported BSON types:
* string
* int (int values will be converted to int32)
* boolean
* double (floats and doubles are the same in Arduino)

### Default values
* string: "NaS"
* int: 0
* bool: false
* float: 0.0

## Classes

- BSONObjBuilder
- BSONObject
- BSONElement
- BSONStreamParser

### BSONObjBuilder Class

The intention of this class is to provide a builder object to create BSON objects, once you have appended data to the builder you can get the BSON Object from it.

**BSONObjBuilder Example:** Build the equivalent BSON object of the following JSON:
```json 
{"hello": "world"} 
```
**Note:** For full example please check the file located on "/examples/BSONObjBuilder"

You need to create a BSON builder and append a string called "hello" with value "world":
```c++
// Create a BSON Builder
BSONObjBuilder bob;
// Append an element to BSON Builder
bob.append("hello", "world");
// Generate BSON Object
BSONObject bo = bob.obj();
```
At this point you will have generated a BSON encoded message like this:
```hex	
\x16\x00\x00\x00                             // total document size
\x02                                         // 0x02 = type String
\x68\x65\x6c\x6c\x6f\x00                     // field name
\x06\x00\x00\x00\x77\x6f\x72\x6c\x64\x00     // field value
\x00                                         // 0x00 = type EOO ('end of object')
```
Once you have obtained a BSON Object from the builder you can access to binary encoded using the "rawData()" method, 
**NOTE:** The method "len()" is useful to retrieve only the encoded part of the BSON object buffer.
```c++
char* data = bo.rawData();
int len = bo.len();
sendDataToValhalla(data, len)
```
### BSONObject Class

This class is used to encode and decode BSON data, can be created using an BSONObjBuilder or constructed using encoded raw data in BSON format.

**BSONObject Example:** Build a BSON Object using BSON encoded raw data.

**Note:** For full example please check the files located on "/examples/BSONObject" and "/examples/jsonString"

```c++
// BSON encoded char array
char bsonRawData[] = {
  0x16, 0x00, 0x00, 0x00,         // Data length
  0x02,                           // Data type
  'h', 'e', 'l', 'l', 'o', 0x00,  // Field name
  0x06, 0x00, 0x00, 0x00,         // String value length (including null terminator)
  'w', 'o', 'r', 'l', 'd', 0x00,  // String value
  0x00                            // EOO
};

// Create BSON Object using raw data
BSONObject bo(bsonRawData);

// Get BSON Object element
BSONElement be = bo.getField("hello");
Serial.print("BSON Element data value: ");
Serial.println(be.getString());
// Print BSON element in JSON format
Serial.print("BSON Object in JSON format: ");
Serial.println(bo.jsonString());
```
The above example should print: 
```
BSON Element data value: world
BSON in JSON format: {"hello": "world"} 
```
