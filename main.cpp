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
    string folder = "CS_Position_Information/";
    string files[] ={
        "CS_Dust_Position_00.csv",
        "CS_Dust_Position_01.csv",
        "CS_Dust_Position_02.csv",
        "CS_Dust_Position_03.csv"
    };

    fstream pos_stream;
    CSVData posData;

    for(unsigned int i = 0; i < 4; ++i){
        pos_stream.open(folder+files[i],fstream::in);
        pos_stream >> posData;
        pos_stream.close();
    }

    cout << "Number of points: " << posData.size() << endl;

    CSVDoubleData posDoubleData = CSVDataToDouble(posData);;
    CSVMatrix<double> positionData;
    positionData.appendMoreData(posDoubleData);

    CSVMatrix<double>::value_vector roundMax = positionData.max();
    CSVMatrix<double>::value_vector roundMin = positionData.min();

    cout << "Position Data MAX: " << roundMax << endl;
    cout << "Position Data Min: " << roundMin << endl;
    cout << "Position Data Average: " << positionData.average() << endl;

    for(unsigned int i = 0; i < roundMax.size(); ++i){
        roundMax[i] = ceil(roundMax[i]/100)*100;
        roundMin[i] = floor(roundMin[i]/100)*100;
    }

    cout << "Round position Data MAX: " << roundMax << endl;
    cout << "Round position Data Min: " << roundMin << endl;

    LN();

    positionData.applyToEachRow(MATH_SUB, roundMin);
    positionData.applyToEachRow(MATH_DIV, applyOperationForEachElement(MATH_SUB,roundMax,roundMin));
    positionData.applyToEachRow(MATH_MUL, vector<double>(roundMax.size(),std::numeric_limits<unsigned short>::max()));

    LN();
    cout << "Round position Data MAX: " << positionData.max() << endl;
    cout << "Round position Data Min: " << positionData.min() << endl;

    fstream shortDataStream(folder+"dataAsUnsignedShort.csv",fstream::out);

    CSVMatrix<unsigned short> shortPositionData;
    for(unsigned int i = 0; i < positionData.data.size(); ++i){
        CSVMatrix<unsigned short>::value_vector tmpRow;
        for(unsigned int j = 0; j < positionData.data[i].size(); ++j){
            tmpRow.push_back(positionData.data[i][j]);
        }
        shortPositionData.data.push_back(tmpRow);
        if(i > 0) shortDataStream << endl;
        shortDataStream << tmpRow;
    }

    LN();
    cout << "Short position Data MAX: " << shortPositionData.max() << endl;
    cout << "Short position Data Min: " << shortPositionData.min() << endl;

    LN();

    fstream combinedData(folder+"combinedData.csv",fstream::out);

    for(unsigned int i = 0; i < shortPositionData.data.size(); ++i){
        if(i > 0) combinedData << endl;
        unsigned char tmp = FIRST4COMB_SHORT(shortPositionData.data[i][0],shortPositionData.data[i][1]);
        combinedData << (unsigned short)tmp << ',';
        tmp = SECOND4COMB_SHORT(shortPositionData.data[i][0],shortPositionData.data[i][1]);
        combinedData << (unsigned short)tmp;
    }

    LN();


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
