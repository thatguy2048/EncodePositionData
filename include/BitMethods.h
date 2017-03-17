#ifndef BITMETHODS_H_INCLUDED
#define BITMETHODS_H_INCLUDED

#define L4B         15
#define F4B         240
#define N4BS        3840
#define F4BS        61440
#define NB(n)       (1 << n)

//get the number of bits in a given type
template<typename T>
inline unsigned int NumberOfBits(){ return sizeof(T)*8; }
//just so it can be called with a value
template<typename T>
inline unsigned int NumberOfBits(const T& t){ return sizeof(T)*8; }

template<typename T, unsigned short MAX, unsigned char SIZE>
inline T GetSegment(const T& value, const short& position){
    return (value>>position*SIZE) & MAX;
}

template<typename T, unsigned short MAX, unsigned char SIZE>
inline T SetSegment(const T& value,const unsigned char& newSegment,const unsigned char& position){
    unsigned char pos = position*SIZE;
    return ((value&(~(MAX<<pos))) | (newSegment<<pos));
}

template<typename T>
inline bool GetBit(const T& value, const unsigned char& bit){
    return GetSegment<T,1,1>(value,bit);
}

template<typename T>
inline T SetBit(const T& value,const bool& bit,const unsigned char& bit_num){
    return SetSegment<T,1,1>(value,bit,bit_num);
}

template<typename T>
inline unsigned char GetCrumb(const T& value, const unsigned char& crumb){
    return GetSegment<T,3,2>(value,crumb);
}

template<typename T>
inline T SetCrumb(const T& value,const unsigned char& crumb,const unsigned char& crumb_num){
    return SetSegment<T,3,2>(value,crumb, crumb_num);
}

template<typename T>
inline unsigned char GetNibble(const T& value, const unsigned char& nibble){
    return GetSegment<T,0xF,4>(value,nibble);
}

template<typename T>
inline T SetNibble(const T& value,const unsigned char& nibble,const unsigned char& nibble_num){
    return SetSegment<T,0xF,4>(value,nibble, nibble_num);
}

template<typename T>
inline unsigned char GetByte(const T& value, const unsigned char& byte){
    return GetSegment<T,0xFF,8>(value,byte);
}

template<typename T>
inline T SetByte(const T& value,const unsigned char& byte,const unsigned char& byte_num){
    return SetSegment<T,0xFF,8>(value,byte, byte_num);
}

template<typename T>
inline unsigned short GetShort(const T& value, const unsigned char& shrt){
    return GetSegment<T,0xFFFF,16>(value,shrt);
}

template<typename T>
inline T SetShort(const T& value,const unsigned char& shrt,const unsigned char& shrt_num){
    return SetSegment<T,0xFFFF,16>(value,shrt, shrt_num);
}


enum BIT_MERGE_TYPE{
    MERGE_BIT,
    MERGE_CRUMB,
    MERGE_NIBBLE,
    MERGE_BYTE,
    MERGE_SHORT,
    MERGE_DEFAULT
};

//get the number of sections in a given value, for the chosen merge type
template<typename T>
unsigned int NumberOfIterationsForMergeType(BIT_MERGE_TYPE mergeType){
    switch(mergeType){
        case MERGE_BIT:     return sizeof(T)*8;
        case MERGE_CRUMB:   return sizeof(T)*4;
        case MERGE_NIBBLE:  return sizeof(T)*2;
        case MERGE_BYTE:    return sizeof(T);
        case MERGE_SHORT:   return sizeof(T)/2;
        default:            return 1;
    }
}

#include <iostream>

template<typename T>
std::ostream& bts(const T& val){
    for(unsigned int i = 8*sizeof(T); i > 0; --i){
        std::cout << GetBit(val,(i-1));
    }
    return std::cout;
}

//The out type should be able to hold the combination of the two input types
template<typename in_type, typename out_type>
out_type MergeBits(const in_type& A, const in_type& B, BIT_MERGE_TYPE mergeType = MERGE_DEFAULT){

    out_type output = 0;
    unsigned int cc = 0;

    for(unsigned int c = NumberOfIterationsForMergeType<in_type>(mergeType); c>0; c--){
        cc = (c*2)-1;

        switch(mergeType){
            case MERGE_BIT:
                output = SetBit(output,GetBit(A,(c-1)),cc);
                output = SetBit(output,GetBit(B,(c-1)),(cc-1));
                break;
            case MERGE_CRUMB:
                output = SetCrumb(output,GetCrumb(A,(c-1)),cc);
                output = SetCrumb(output,GetCrumb(B,(c-1)),(cc-1));
                break;
            case MERGE_NIBBLE:
                output = SetNibble(output,GetNibble(A,(c-1)),cc);
                output = SetNibble(output,GetNibble(B,(c-1)),(cc-1));
                break;
            case MERGE_BYTE:
                output = SetByte(output,GetByte(A,(c-1)),cc);
                output = SetByte(output,GetByte(B,(c-1)),(cc-1));
                break;
            case MERGE_SHORT:
                output = SetShort(output,GetShort(A,(c-1)),cc);
                output = SetShort(output,GetShort(B,(c-1)),(cc-1));
                break;
            default:
                output = A << NumberOfBits<in_type>() | B;
                break;
        }
    }
    return output;
}

//Combine the first 4 bits of a and b
#define FIRST4COMB(a,b) ((a&F4B)|(b>>4))
//combine the last 4 bits of a and b
#define LAST4COMB(a,b)  ((a<<4)|(b&L4B))

//combine the first 4 bits of two shorts
#define FIRST4COMB_SHORT(a,b) FIRST4COMB((a>>8),(b>>8))
//combine the second 4 bits of two shorts
#define SECOND4COMB_SHORT(a,b) LAST4COMB((a>>8),(b>>8))

//get most significant
#define MSShort(value)   (value>>(((sizeof(value)/2)-1)*16))
#define MSByte(value)   (value>>((sizeof(value)-1)*8))
#define MSNibble(value) (MSByte(value)>>4)
#define MSCrumb(value)  (MSNibble(value)>>2)
#define MSBit(value)    (MSCrumb(value)>>1)

#endif // BITMETHODS_H_INCLUDED
