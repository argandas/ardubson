#include "IEEE754tools.h"



//
// DEBUG FUNCTIONS
//
// print float components
void dumpFloat(float number)
{
    IEEEfloat* x = (IEEEfloat*) ((void*)&number);
    Serial.print(x->s, HEX);
    Serial.print("\t");
    Serial.print(x->e, HEX);
    Serial.print("\t");
    Serial.println(x->m, HEX);

    // Serial.print(" sign: "); Serial.print(x->s);
    // Serial.print("  exp: "); Serial.print(x->e);
    // Serial.print(" mant: "); Serial.println(x->m);
}

// print "double" components
void dumpDBL(struct _DBL dbl)
{
    Serial.print(dbl.s, HEX);
    Serial.print("\t");
    Serial.print(dbl.e, HEX);
    Serial.print("\t");
    Serial.println(dbl.m, HEX);
}

//
// mapping to/from 64bit double - best effort
//

// converts a float to a packed array of 8 bytes representing a 64 bit double
// restriction exponent and mantisse.
// float;  array of 8 bytes;  LSBFIRST; MSBFIRST
void float2DoublePacked(float number, byte* bar, int byteOrder)
{
    _FLOATCONV fl;
    fl.f = number;
    fl.p = IEEEfloat();
    _DBLCONV dbl;
    dbl.p.filler = 0;
    dbl.p.s = fl.p.s;
    dbl.p.e = fl.p.e - 127 + 1023;  // exponent adjust
    dbl.p.m = fl.p.m;

#ifdef IEEE754_ENABLE_MSB
    if (byteOrder == LSBFIRST)
    {
#endif
        for (int i = 0; i < 8; i++)
        {
            bar[i] = dbl.b[i];
        }
#ifdef IEEE754_ENABLE_MSB
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            bar[i] = dbl.b[7-i];
        }
    }
#endif
}

// converts a packed array of bytes into a 32bit float.
// there can be an exponent overflow
// the mantisse is truncated to 23 bits.
float doublePacked2Float(byte* bar, int byteOrder)
{
    _FLOATCONV fl;
    _DBLCONV dbl;

#ifdef IEEE754_ENABLE_MSB
    if (byteOrder == LSBFIRST)
    {
#endif
        for (int i = 0; i < 8; i++)
        {
            dbl.b[i] = bar[i];
        }
#ifdef IEEE754_ENABLE_MSB
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            dbl.b[i] = bar[7-i];
        }
    }
#endif

    int e = dbl.p.e - 1023 + 127;  // exponent adjust
    // TODO check exponent overflow.
    if (e >=0 || e <= 255)
    {
        fl.p.s = dbl.p.s;
        fl.p.e = e;
        fl.p.m = dbl.p.m;  // note this one clips the mantisse
        return fl.f;
    }
    return NAN;  // OR +-INF?
    // return (fl.p.s) ? -INFINITY : INFINITY;
}

//
// TEST FUNCTIONS
//

// ~1.7x faster
int IEEE_NAN(float number)
{
    uint16_t* x = ((uint16_t*) &number + 1);
    return ((*x) == 0x7FC0);
}

// ~3.4x faster
int IEEE_INF(float number)
{
    uint8_t* x = ((uint8_t*) &number);
    if (*(x+2) != 0x80) return 0;
    if (*(x+3) == 0x7F) return 1;
    if (*(x+3) == 0xFF) return -1;
    return 0;
}

// for the real speed freaks, the next two
boolean IEEE_PosINF(float number)
{
    return (* ((uint16_t*) &number + 1) ) == 0x7F80;
}

boolean IEEE_NegINF(float number)
{
    return (* ((uint16_t*) &number + 1) ) == 0xFF80;
}




//
// PROPERTIES
//
uint8_t IEEE_Sign(float number)
{
  IEEEfloat* x = (IEEEfloat*) ((void*)&number);
  return x->s;
}

int IEEE_Exponent(float number)
{
  IEEEfloat* x = (IEEEfloat*) ((void*)&number);
  return x->e - 127;
}

uint32_t IEEE_Mantisse(float number)
{
  IEEEfloat* x = (IEEEfloat*) ((void*)&number);
  return x->m;
}

//
// MATH FUNCTIONS
//

// factor ~2.7; (tested with *16) more correct than the faster one
float IEEE_POW2(float number, int n)
{
    _FLOATCONV fl;
    fl.f = number;
    int e = fl.p.e + n;
    if (e >= 0 && e < 256)
    {
        fl.p.e = e;
        return fl.f;
    }
    return (fl.p.s) ? -INFINITY : INFINITY;
}

// WARNING no overflow detection in the SHIFT (factor ~3.5)
float IEEE_POW2fast(float number, int n)
{
    _FLOATCONV fl;
    fl.f = number;
    fl.p.e += n;
    return fl.f;
}



//
// NOT TESTED FUNCTIONS
//

//
// get truncated part as separate float.
//
void doublePacked2Float2(byte* bar, int byteOrder, float* value, float* error)
{
    _FLOATCONV fl;
    _DBLCONV dbl;

#ifdef IEEE754_ENABLE_MSB
    if (byteOrder == LSBFIRST)
    {
#endif
        for (int i = 0; i < 8; i++)
        {
            dbl.b[i] = bar[i];
        }
#ifdef IEEE754_ENABLE_MSB
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            dbl.b[i] = bar[7 - i];
        }
    }
#endif

    int e = dbl.p.e - 1023 + 127;  // exponent adjust
    // TODO check exponent overflow.
    if (e >=0 || e <= 255)
    {
        fl.p.s = dbl.p.s;
        fl.p.e = e;
        fl.p.m = dbl.p.m;  // note this one clips the mantisse
        *value = fl.f;

        fl.p.s = dbl.p.s;
        fl.p.e = e-23;
        fl.p.m = dbl.p.filler;  // note this one clips the mantisse
        *error = fl.f;
    }
    *value = (dbl.p.s) ? -INFINITY : INFINITY;
    *error = 0;
}


// what is this???
float IEEE_FLIP(float number)
{
    _FLOATCONV fl;
    fl.f = number;
    fl.p.e = -fl.p.e;
    fl.p.m = (0x007FFFFF - fl.p.m);
    return fl.f;
}

/*
// ONELINERS to speed up some specific 32 bit float math

// *(((byte*) &number)+3) &= 0x7F;              // number == fabs(number);
// x = *(((byte*) &number)+3) & 0x7F;           // x = fabs(number);
// GAIN = factor 2

// *(((byte*) &number)+3) |= 0x80;              // number == -fabs(number);
// x = *(((byte*) &number)+3) | 0x80;           // x == -fabs(number);
// GAIN = factor 2

// *(((byte*) &number)+3) ^= 0x80;              // number = -number;
// x = *(((byte*) &number)+3) ^ 0x80;           // x = -number;
// GAIN = factor 2

// s = *(((uint8_t*) &number)+3) & 0x80;        // s = sign(number);
// if ( *(((byte*) &number)+3) & 0x80) x=2;     // if (number < 0) x=2;
// GAIN = factor 5


// no speed optimize found for

boolean IEEE_ZERO(float number)
{
    return (* ((uint32_t*) &number) ) & 0x7FFFFFFF;
}

float IEEE_DIV2(float number)
{
    IEEEfloat* x = (IEEEfloat*) ((void*)&number);
     x->e--;
    return number;
}

bool IEEE_LESS(float f, float g)
{
  IEEEfloat* x = (IEEEfloat*) ((void*)&f);
  IEEEfloat* y = (IEEEfloat*) ((void*)&g);
  if (x->s > y->s) return 1;
  if (x->s < y->s) return 0;
  if (x->e < y->e) return 1;
  if (x->e > y->e) return 0;
  if (x->m < y->m) return 1;
  return 0;
}

bool IEEE_EQ(float f, float g)
{
  IEEEfloat* x = (IEEEfloat*) ((void*)&f);
  IEEEfloat* y = (IEEEfloat*) ((void*)&g);
  return (x->m == y->m) && (x->e == y->e) && (x->s != y->s);
}

*/
