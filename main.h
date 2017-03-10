#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <iostream>
#include "CSVData.h"
#include <fstream>
#include <cmath>
#include "BitMethods.h"

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
        pos_stream.open(folder+files[i],std::fstream::in);
        pos_stream >> posData;
        pos_stream.close();
    }

    CSVDoubleData posDoubleData = CSVDataToDouble(posData);;
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

    std::fstream shortDataStream(folder+"dataAsUnsignedShort.csv",std::fstream::out);

    CSVMatrix<unsigned short> shortPositionData;
    for(unsigned int i = 0; i < positionData.data.size(); ++i){
        CSVMatrix<unsigned short>::value_vector tmpRow;
        for(unsigned int j = 0; j < positionData.data[i].size(); ++j){
            tmpRow.push_back(positionData.data[i][j]);
        }
        shortPositionData.data.push_back(tmpRow);
        if(i > 0) shortDataStream << std::endl;
        shortDataStream << tmpRow;
    }

    std::fstream combinedData(folder+"combinedData.csv",std::fstream::out);

    for(unsigned int i = 0; i < shortPositionData.data.size(); ++i){
        if(i > 0) combinedData << std::endl;
        unsigned char tmp = FIRST4COMB_SHORT(shortPositionData.data[i][0],shortPositionData.data[i][1]);
        combinedData << (unsigned short)tmp << ',';
        tmp = SECOND4COMB_SHORT(shortPositionData.data[i][0],shortPositionData.data[i][1]);
        combinedData << (unsigned short)tmp;
    }
}

#endif // MAIN_H_INCLUDED
