//
//    FILE: IEEE754tools.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.1
// PURPOSE: manipulate IEEE754 float numbers fast
//     URL: https://github.com/RobTillaart/IEEE754tools.git
//
//  EXPERIMENTAL ==> USE WITH CARE
//  not tested extensively,
//
//  0.1.00  2013-09-08  initial version
//  0.1.01  2013-09-08  added IEEE_NAN, IEEE_INF tests + version string
//  0.1.02  2013-09-08  added SHIFT_POW2
//  0.1.03  2013-09-10  renamed IEEE_Sign IEEE_Exponent
//  0.2.0   2020-06-30  own repo + some refactor...
//  0.2.1   2020-12-30  arduino-CI

#ifndef IEEE754tools_h
#define IEEE754tools_h

#include "Arduino.h"

#define IEEE754_VERSION "0.2.1"

// (un)comment lines to configure functionality / size
//#define IEEE754_ENABLE_MSB   // +78 bytes


// IEEE754 float layout;
struct IEEEfloat
{
    uint32_t m:23;
    uint8_t e:8;
    uint8_t s:1;
};

// IEEE754 double layout;
struct IEEEdouble
{
    uint64_t m:52;
    uint16_t e:11;
    uint8_t s:1;
};

// Arduino UNO double layout:
// the UNO has no 64 bit double, it is only able to map 23 bits of the mantisse
// a filler is added for the remaining bits. These might be useful in future?
struct _DBL
{
    uint32_t filler:29;
    uint32_t m:23;
    uint16_t e:11;
    uint8_t  s:1;
};

// for packing and unpacking a float
union _FLOATCONV
{
    IEEEfloat p;
    float f;
    byte b[4];
};

// for packing and unpacking a double
union _DBLCONV
{
    // IEEEdouble p;
    _DBL p;
    double d;           // !! is a 32bit float for UNO.
    byte b[8];
};

//
// DEBUG FUNCTIONS
//
// print float components
void dumpFloat(float number);

// print "double" components
void dumpDBL(struct _DBL dbl);

//
// mapping to/from 64bit double - best effort
//

// converts a float to a packed array of 8 bytes representing a 64 bit double
// restriction exponent and mantisse.
// float;  array of 8 bytes;  LSBFIRST; MSBFIRST
void float2DoublePacked(float number, byte* bar, int byteOrder = LSBFIRST);

// converts a packed array of bytes into a 32bit float.
// there can be an exponent overflow
// the mantisse is truncated to 23 bits.
float doublePacked2Float(byte* bar, int byteOrder = LSBFIRST);

//
// TEST FUNCTIONS
//

// ~1.7x faster
int IEEE_NAN(float number);

// ~3.4x faster
int IEEE_INF(float number);

// for the real speed freaks, the next two
boolean IEEE_PosINF(float number);

boolean IEEE_NegINF(float number);




//
// PROPERTIES
//
uint8_t IEEE_Sign(float number);

int IEEE_Exponent(float number);

uint32_t IEEE_Mantisse(float number);

//
// MATH FUNCTIONS
//

// factor ~2.7; (tested with *16) more correct than the faster one
float IEEE_POW2(float number, int n);

// WARNING no overflow detection in the SHIFT (factor ~3.5)
float IEEE_POW2fast(float number, int n);



//
// NOT TESTED FUNCTIONS
//

//
// get truncated part as separate float.
//
void doublePacked2Float2(byte* bar, int byteOrder, float* value, float* error);


// what is this???
float IEEE_FLIP(float number);

#endif
// -- END OF FILE --
