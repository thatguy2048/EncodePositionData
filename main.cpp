#include "main.h"
#include <bitset>

#define LN() cout << endl
#define LOG(x) cout << x << endl

using namespace std;

template<typename T>
std::ostream& bitsToStream(std::ostream& os, const T& val){
    for(unsigned int i = 8*sizeof(T); i > 0; --i){
        os << getBit(val,(i-1));
    }
    return os;
}

template<typename T>
void printBits(const T& s){
    bitsToStream(cout, s);
}

int main(){
    preProcessPositionData();


    /*
    cout << "Print Bits" << endl;

    unsigned char tcs1 = 202;
    unsigned char tcs2 = 109;

    cout << endl << "Combine Bit Test" << endl << "Original\t A: ";
    printBits(tcs1);
    cout << "\t B: ";
    printBits(tcs2);
    cout << "\t" << (unsigned char)tcs1 << "\t" << (unsigned char)tcs2;
    LN();

    cout << "Combined\tCA: ";
    printBits<unsigned char>(FIRST4COMB(tcs1,tcs2));
    cout << "\tCB: ";
    printBits<unsigned char>(LAST4COMB(tcs1,tcs2));
    cout << "\t" << (unsigned char)FIRST4COMB(tcs1,tcs2) << "\t" << (unsigned char)LAST4COMB(tcs1,tcs2);
    LN();
    cout << "ReCombined\t A: ";
    printBits<unsigned char>(FIRST4COMB(FIRST4COMB(tcs1,tcs2),LAST4COMB(tcs1,tcs2)));
    cout << "\t B: ";
    printBits<unsigned char>(LAST4COMB(FIRST4COMB(tcs1,tcs2),LAST4COMB(tcs1,tcs2)));
    cout << "\t" << (unsigned char)FIRST4COMB(FIRST4COMB(tcs1,tcs2),LAST4COMB(tcs1,tcs2)) << "\t" << (unsigned char)LAST4COMB(FIRST4COMB(tcs1,tcs2),LAST4COMB(tcs1,tcs2));
    LN();
    */
    cout << endl << "DONE...";
    cin.ignore();
    return 0;
}
