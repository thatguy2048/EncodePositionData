#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <limits>
#include "VectorMethods.h"

typedef std::vector< std::vector<std::string> > CSVData;
typedef std::vector< std::vector<double> > CSVDoubleData;

//Parse a stream, and add it to the provided CSVData
CSVData& CSVDataFromStream(std::istream& in_str, CSVData& data){
    std::string line;
    while(std::getline(in_str,line,'\n')){
        data.push_back(SplitString(line,','));
    }
    return data;
}

//parse the data in the stream into CSVData
CSVData CSVDataFromStream(std::istream& in_str){
    CSVData output;
    CSVDataFromStream(in_str, output);
    return output;
}

//Read the csv data from a stream
std::istream& operator>>(std::istream& is, CSVData& output){
    CSVDataFromStream(is, output);
    return is;
}

//Write the csv data back to a stream
std::ostream& operator<<(std::ostream& os, const CSVData& data){
    return WriteVecToStream(os,data,'\n');
}

CSVDoubleData CSVDataToDouble(const CSVData& data){
    CSVDoubleData output;
    for(unsigned int i = 0; i < data.size(); ++i){
        std::vector<double> currentRow;
        for(unsigned int j = 0; j < data[i].size(); j++){
            currentRow.push_back(atof(data[i][j].c_str()));
        }
        output.push_back(currentRow);
    }
    return output;
}

//Write the csv double data to a stream
std::ostream& operator<<(std::ostream& os, const CSVDoubleData& data){
    return WriteVecToStream(os,data,'\n');
}

template<typename T>
class CSVMatrix{
public:
    typedef T value_type;
    typedef std::vector<value_type> value_vector;
    typedef std::vector< value_vector > data_type;

protected:

public:
    data_type data;
    value_type MAX_VALUE;

    CSVMatrix(){
        MAX_VALUE = std::numeric_limits<value_type>::max();
    }

    void appendMoreData(const data_type& moreData){
        data.insert(data.end(), moreData.begin(), moreData.end());
    }

    value_vector max(){
        value_vector output;
        for(unsigned int i = 0; i< data.size(); ++i){
            while(output.size() < data[i].size())   output.push_back(0);
            for(unsigned int j = 0; j < data[i].size(); ++j){
                if(output[j] < data[i][j])  output[j] = data[i][j];
            }
        }
        return output;
    }

    value_vector min(){
        value_vector output;
        for(unsigned int i = 0; i< data.size(); ++i){
            while(output.size() < data[i].size())   output.push_back(MAX_VALUE);
            for(unsigned int j = 0; j < data[i].size(); ++j){
                if(output[j] > data[i][j])  output[j] = data[i][j];
            }
        }
        return output;
    }

    value_vector average(){
        value_vector output;
        for(unsigned int i = 0; i< data.size(); ++i){
            while(output.size() < data[i].size())   output.push_back(0);
            for(unsigned int j = 0; j < data[i].size(); ++j){
                output[j] += data[i][j];
            }
        }

        for(unsigned int j = 0; j < output.size(); ++j){
            output[j] /= data.size();
        }
        return output;
    }

    void applyToEachRow(MathOperation op, const value_vector& values){
        data = applyOperationToEachRow(op, data, values);
    }
};

#endif // CSVPARSER_H
