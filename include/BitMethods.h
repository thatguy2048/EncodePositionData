#ifndef BITMETHODS_H_INCLUDED
#define BITMETHODS_H_INCLUDED

#include <ostream>

#define L4B         15
#define F4B         240
#define N4BS        3840
#define F4BS        61440
#define NB(n)       (1 << n)
#define getBit(v,b) ((v >> b) & 1)

//Combine the first 4 bits of a and b
#define FIRST4COMB(a,b) ((a&F4B)|(b>>4))
//combine the last 4 bits of a and b
#define LAST4COMB(a,b)  ((a<<4)|(b&L4B))

//combine the first 4 bits of two shorts
#define FIRST4COMB_SHORT(a,b) FIRST4COMB((a>>8),(b>>8))
//combine the second 4 bits of two shorts
#define SECOND4COMB_SHORT(a,b) LAST4COMB((a>>8),(b>>8))

#endif // BITMETHODS_H_INCLUDED
