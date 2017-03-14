#ifndef BITMETHODS_H_INCLUDED
#define BITMETHODS_H_INCLUDED

#include <ostream>

#define L4B         15
#define F4B         240
#define N4BS        3840
#define F4BS        61440
#define NB(n)       (1 << n)
#define NumberOfBitsInType(type) (sizeof(type)*8)
#define GetBit(number, bitNumber) ((number >> bitNumber) & 1)
#define SetBit(number, bitValue, bitNumber) (number ^= (-bitValue ^ number) & (1 << bitNumber))

#define GetCrumb(v,c)  ((v&(3<<c*2)) >> (c*2))
#define SetCrumb(value, crumb, n) (value = ((value&(~(3<<n*2))) | (crumb<<n*2)))

#define GetNibble(v,n)  ((v&(0xF<<n*4)) >> (n*4))
#define SetNibble(value, nibble, n) (value = ((value&(~(0xF<<n*4))) | (nibble<<n*4)))

#define GetByte(v,b)    ((v&(0xFF<<b*8)) >> (b*8))
#define SetByte(value, byte, b) (value = ((value&(~(0xFF<<b*8))) | (byte<<b*8)))

//Combine the first 4 bits of a and b
#define FIRST4COMB(a,b) ((a&F4B)|(b>>4))
//combine the last 4 bits of a and b
#define LAST4COMB(a,b)  ((a<<4)|(b&L4B))

//combine the first 4 bits of two shorts
#define FIRST4COMB_SHORT(a,b) FIRST4COMB((a>>8),(b>>8))
//combine the second 4 bits of two shorts
#define SECOND4COMB_SHORT(a,b) LAST4COMB((a>>8),(b>>8))

#endif // BITMETHODS_H_INCLUDED
