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
    std::fstream shortOutStream(DATA_FOLDER "bitCombinedData_short.csv", std::fstream::out);
    CSVData posData;
    posStream >> posData;
    posStream.close();

    VECTOR_MAT(unsigned short) shortData = CSVDataToType<unsigned short>(posData);

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
        unsigned int crumbComb = 0;
        for(unsigned int c = 8; c > 0; c--){
            unsigned int cc = (c*2)-1;
            SetCrumb(crumbComb,GetCrumb(A,(c-1)),cc);
            SetCrumb(crumbComb,GetCrumb(B,(c-1)),(cc-1));
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

        //set short
        unsigned int shortComb = (A<<16) | B;

        if(i > 0){
                outStream << std::endl;
                shortOutStream << std::endl;
        }
        outStream << bitComb << "," << crumbComb << "," << nibbleComb << "," << byteComb << "," << shortComb;
        shortOutStream << (bitComb>>16) << "," << (crumbComb>>16) << "," << (nibbleComb>>16) << "," << (byteComb>>16) << "," << (shortComb>>16);
    }
}

struct EncodingResult{
    unsigned int totalSize;
    unsigned int numberOfBitsEncoded;
    int mostCommon = {-1};

    EncodingResult(unsigned int TotalSize = 0, unsigned int NumberOfBitsEncoded = 0):totalSize(TotalSize), numberOfBitsEncoded(NumberOfBitsEncoded){}

    unsigned int totalBits() const{   return totalSize*8;    }
    unsigned int bitSavings() const{  return totalBits()-numberOfBitsEncoded; }
    double savingsPercent() const{    return 100*bitSavings()/(double)totalBits(); }
    double savingsPerMessage() const{ return bitSavings()/(double)totalSize;  }

    friend std::ostream& operator<<(std::ostream& os, const EncodingResult& rslt){
        return os << "Original Size: " << rslt.totalBits() << "\t Encoded Size: " << rslt.numberOfBitsEncoded << "\t SPM: " << rslt.savingsPerMessage();
    }
};

template<typename T>
EncodingResult TestEncodingData(const std::vector<T> data){
    typedef basic_probabilityModel<unsigned int, T, std::numeric_limits<T>::max()> TmpProbabilityModel;

    EncodingResult output;

    TmpProbabilityModel model;
    Arithmetic::Encoder<TmpProbabilityModel> encoder(&model);

    for(unsigned int i = 0; i < data.size(); ++i){  model.countValue(data[i]);  }
    for(unsigned int i = 0; i < data.size(); ++i){  encoder.encodeSymbol(data[i]);  }

    output.totalSize = data.size();
    BitStream bs = encoder.endEncoding();
    output.numberOfBitsEncoded = bs.numberOfBits();
/*
    std::cout << "Most Probable: " << (int)model.getMostProbable() << "\t";
    printBits(model.getMostProbable());
    std::cout << std::endl;
*/
    output.mostCommon = model.getMostProbable();

    return output;
}

void TestCombinedData(){
    std::fstream pos_stream(DATA_FOLDER "bitCombinedData.csv", std::fstream::in);
    CSVData pos_data;
    pos_stream >> pos_data;
    pos_stream.close();

    VECTOR_MAT(unsigned int) positionData = CSVDataToType<unsigned int>(pos_data);

    for(unsigned int comb = 0; comb < positionData[0].size(); comb++){
        std::vector<unsigned char> dataToEncode;

        for(unsigned int i = 0; i < positionData.size(); ++i){
            dataToEncode.push_back( GetByte(positionData[i][comb],3) >> 6 );
        }
        std::cout << comb << "\t" << TestEncodingData(dataToEncode) << std::endl;
    }
}

void TestCombinedDataCrumb(){
    std::fstream pos_stream(DATA_FOLDER "bitCombinedData.csv", std::fstream::in);
    CSVData pos_data;
    pos_stream >> pos_data;
    pos_stream.close();

    VECTOR_MAT(unsigned int) positionData = CSVDataToType<unsigned int>(pos_data);

    unsigned int previousMostCommon = 0;
    std::vector<unsigned int> pData;
    std::vector<unsigned int> previousData;

    std::cout << std::endl;
    for(unsigned int crumb = sizeof(unsigned int)*4; crumb >= sizeof(unsigned int)*3+1; crumb--){
        std::vector<unsigned char> dataToEncode;
        unsigned int crmb = crumb-1;

        if(previousData.empty()){
            for(unsigned int i = 0; i < positionData.size(); ++i){
                dataToEncode.push_back( GetCrumb(positionData[i][0],crmb) );

            }
        }else{
            for(unsigned int i = 0; i < previousData.size(); ++i){
                if(previousMostCommon == GetCrumb(previousData[i],(crmb+1) )){
                    dataToEncode.push_back( GetCrumb(previousData[i],crmb) );
                    pData.push_back(previousData[i]);
                }
            }
        }

        previousData = pData;
        pData.clear();

        EncodingResult rslt = TestEncodingData(dataToEncode);
        previousMostCommon = rslt.mostCommon;
        std::cout << crmb << "\t" << rslt << std::endl;
    }
}

void TestDataCombineMethods(){
    std::cout << "Read Data From File: '" << "bitCombinedData.csv" << "'" << std::endl;
    std::fstream pos_stream(DATA_FOLDER "bitCombinedData.csv", std::fstream::in);
    CSVData pos_data;
    pos_stream >> pos_data;
    pos_stream.close();

    VECTOR_MAT(unsigned int) positionData = CSVDataToType<unsigned int>(pos_data);

    std::cout << "Shuffle Data" << std::endl;
    shuffleVector(positionData);

    std::cout << "Split Data" << std::endl;
    unsigned int splitAbout = Lerp<unsigned int>(0,positionData.size(),0.9);



    std::vector<EncodingResult> results;

    std::cout << std::endl;
    for(unsigned int comb = 0; comb < positionData[0].size(); comb++){
        std::cout << "Testing Comb Type " << comb << std::endl;
        for(unsigned int byte = 3; byte >= 2; byte--){
            std::cout << "Testing Byte " << byte  << std::endl;
            EncodingResult result;

            ProbabilityModel model(true);
            Arithmetic::Encoder<ProbabilityModel> encoder(&model);

            for(unsigned int i = 0; i < splitAbout; ++i){
                model.countValue(GetByte(positionData[i][comb],byte));
            }

            for(unsigned int i = splitAbout; i < positionData.size(); ++i){
                encoder.encodeSymbol(GetByte(positionData[i][comb],byte));
            }

            BitStream encoded_stream = encoder.endEncoding();

            result.totalSize = positionData.size()-splitAbout;
            std::cout << "Total Number Of Bits: " << result.totalBits() << std::endl;
            result.numberOfBitsEncoded = encoded_stream.numberOfBits();
            std::cout << "Bitstream Size:       " << result.numberOfBitsEncoded << std::endl;;
            std::cout << "Bit Savings           " << result.bitSavings() << " bits\t" << result.savingsPercent() << "%\t" << result.savingsPerMessage() << std::endl;

            std::cout << std::endl;

            results.push_back(result);
        }
    }

    std::cout << "Results" << std::endl;

    for(unsigned int i = 0; i < positionData[0].size(); ++i){
        std::cout << i << "\t" << results[i*2] << std::endl;
        std::cout << i << ".5\t" << results[i*2+1] << std::endl;
        std::cout << "Total SPM:\t" << results[i*2+1].savingsPerMessage()+results[i*2].savingsPerMessage() << std::endl;
    }
}

#endif // MAIN_H_INCLUDED
