#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <iostream>
#include "CSVData.h"
#include <fstream>
#include <cmath>
#include "BitMethods.h"
#include <algorithm>
#include <ctime>
#include "Encoder.h"
#include "Decoder.h"

#define DATA_FOLDER         "CS_Position_Information/"
#define COMBINED_DATA_FILE  "combinedData.csv"

template<typename T>
std::ostream& bitsToStream(std::ostream& os, const T& val){
    for(unsigned int i = 8*sizeof(T); i > 0; --i){
        os << GetBit(val,(i-1));
    }
    return os;
}

template<typename T>
void printBits(const T& s){
    bitsToStream(std::cout, s);
}

template<typename T>
T Lerp(const T& start, const T& finished, float r){
    return start+r*(finished-start);
}

void preProcessPositionData(){
    std::string folder = "CS_Position_Information/";
    std::string files[] ={
        "CS_Dust_Position_00.csv",
        "CS_Dust_Position_01.csv",
        "CS_Dust_Position_02.csv",
        "CS_Dust_Position_03.csv"
    };

    std::fstream pos_stream;
    CSVData posData;

    for(unsigned int i = 0; i < 4; ++i){
        pos_stream.open(DATA_FOLDER+files[i],std::fstream::in);
        pos_stream >> posData;
        pos_stream.close();
    }

    VECTOR_MAT(double) posDoubleData = CSVDataToType<double>(posData);
    CSVMatrix<double> positionData;
    positionData.appendMoreData(posDoubleData);

    CSVMatrix<double>::value_vector roundMax = positionData.max();
    CSVMatrix<double>::value_vector roundMin = positionData.min();

    for(unsigned int i = 0; i < roundMax.size(); ++i){
        roundMax[i] = ceil(roundMax[i]/100)*100;
        roundMin[i] = floor(roundMin[i]/100)*100;
    }

    positionData.applyToEachRow(MATH_SUB, roundMin);
    positionData.applyToEachRow(MATH_DIV, applyOperationForEachElement(MATH_SUB,roundMax,roundMin));
    positionData.applyToEachRow(MATH_MUL, std::vector<double>(roundMax.size(),std::numeric_limits<unsigned short>::max()));

    std::fstream shortDataStream(DATA_FOLDER "dataAsUnsignedShort.csv",std::fstream::out);

    CSVMatrix<unsigned short> shortPositionData;
    for(unsigned int i = 0; i < positionData.size(); ++i){
        CSVMatrix<unsigned short>::value_vector tmpRow;
        for(unsigned int j = 0; j < positionData[i].size(); ++j){
            tmpRow.push_back(positionData[i][j]);
        }
        shortPositionData.appendRow(tmpRow);
        if(i > 0) shortDataStream << std::endl;
        shortDataStream << tmpRow;
    }

    std::fstream combinedData(DATA_FOLDER COMBINED_DATA_FILE,std::fstream::out);

    for(unsigned int i = 0; i < shortPositionData.size(); ++i){
        if(i > 0) combinedData << std::endl;
        unsigned char tmp = FIRST4COMB_SHORT(shortPositionData[i][0],shortPositionData[i][1]);
        combinedData << (unsigned short)tmp << ',';
        tmp = SECOND4COMB_SHORT(shortPositionData[i][0],shortPositionData[i][1]);
        combinedData << (unsigned short)tmp;
    }
}

VECTOR_MAT(unsigned char) readCombinedDataFromFile(){
    std::fstream pos_stream(DATA_FOLDER COMBINED_DATA_FILE, std::fstream::in);
    CSVData posData;

    pos_stream >> posData;

    pos_stream.close();

    return CSVDataToType<unsigned char>(posData);
}

template<typename T>
void shuffleVector(VECTOR_MAT(T)& mat){
    std::srand(unsigned (std::time(0)));
    std::random_shuffle(mat.begin(), mat.end());
}

//combine the x and y data by bit, nibble, and byte, and write the result to a file.
void CreateNewCombinedData(){
    std::fstream posStream(DATA_FOLDER "dataAsUnsignedShort.csv", std::fstream::in);
    std::fstream outStream(DATA_FOLDER "bitCombinedData.csv", std::fstream::out);
    CSVData posData;
    posStream >> posData;
    posStream.close();

    VECTOR_MAT(unsigned short) shortData = CSVDataToType<unsigned short>(posData);

    std::vector<unsigned int> combinedData;
    for(unsigned int i = 0; i < shortData.size(); ++i){
        unsigned short A = shortData[i][0];
        unsigned short B = shortData[i][1];

        //set bits
        unsigned int bitComb = 0;
        for(unsigned int b = 16; b > 0; b--){
            unsigned int cb = (b*2)-1;
            SetBit(bitComb,GetBit(A,(b-1)),cb);
            SetBit(bitComb,GetBit(B,(b-1)),(cb-1));
        }

        //set nibbles
        unsigned int nibbleComb = 0;
        for(unsigned int n = 4; n > 0; n--){
            unsigned int cn = (n*2)-1;
            SetNibble(nibbleComb,GetNibble(A,(n-1)),cn);
            SetNibble(nibbleComb,GetNibble(B,(n-1)),(cn-1));
        }

        //set bytes
        unsigned int byteComb = 0;
        for(unsigned int b = 2; b > 0; b--){
            unsigned int bn = (b*2)-1;
            SetByte(byteComb,GetByte(A,(b-1)),bn);
            SetByte(byteComb,GetByte(B,(b-1)),(bn-1));
        }

        combinedData.push_back(bitComb);

        if(i > 0)   outStream << std::endl;
        outStream << bitComb << "," << nibbleComb << "," << byteComb;
    }
}

#endif // MAIN_H_INCLUDED
