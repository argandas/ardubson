# ardubson
Lightweight BSON Library for Arduino

## Status

This is a work in progress, please download the library from the [releases](https://github.com/argandas/ardubson/releases) page.

## Getting started

Please check the examples folder for detailed explanations

## Supported BSON types:
* string
* int (int values will be converted to int32)
* boolean

### Default values
* string: "NaS"
* int: 0
* bool: false

## Classes

- BSONObjBuilder
- BSONObject
- BSONElement

### BSONObjBuilder

The intention of this class is to provide a builder object to create BSON objects, once you have appended data to the builder you can get the BSON Object from it.

Example: To get the equivalent BSON object of the following JSON:
```json 
{"hello": "world"} 
```
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
```
  char* data = bo.rawData();
  int len = bo.len();
  sendDataToValhalla(data, len)
```
