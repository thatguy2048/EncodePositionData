#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <limits>
#include "VectorMethods.h"

#define VECTOR_MAT(type) std::vector< std::vector<type> >

typedef VECTOR_MAT(std::string) CSVData;

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

enum CONVERSION_TYPE{
    CONV_INT,
    CONV_FLOAT,
    CONV_LONG,
    CONV_LONG_LONG
};

template<typename T, CONVERSION_TYPE ct>
VECTOR_MAT(T) ConvertCSVDataToType(const CSVData& data){
    VECTOR_MAT(T) output;
    for(unsigned int i = 0; i < data.size(); ++i){
        std::vector<T> currentRow;
        for(unsigned int j = 0; j < data[i].size(); j++){
            switch(ct){
                case CONV_FLOAT:
                    currentRow.push_back(atof(data[i][j].c_str()));
                    break;
                case CONV_LONG:
                    currentRow.push_back(atol(data[i][j].c_str()));
                    break;
                case CONV_LONG_LONG:
                    currentRow.push_back(atoll(data[i][j].c_str()));
                    break;
                default:
                    currentRow.push_back(atoi(data[i][j].c_str()));
                    break;
            }
        }
        output.push_back(currentRow);
    }
    return output;
}

template<typename T>
VECTOR_MAT(T) CSVDataToType(const CSVData& data){
    return ConvertCSVDataToType<T,CONV_INT>(data);
}

template<>
VECTOR_MAT(float) CSVDataToType<float>(const CSVData& data){
    return ConvertCSVDataToType<float,CONV_FLOAT>(data);
}

template<>
VECTOR_MAT(double) CSVDataToType<double>(const CSVData& data){
    return ConvertCSVDataToType<double,CONV_FLOAT>(data);
}

template<>
VECTOR_MAT(long int) CSVDataToType<long int>(const CSVData& data){
    return ConvertCSVDataToType<long int,CONV_LONG>(data);
}

template<>
VECTOR_MAT(unsigned long int) CSVDataToType<unsigned long int>(const CSVData& data){
    return ConvertCSVDataToType<unsigned long int,CONV_LONG>(data);
}

template<>
VECTOR_MAT(long long int) CSVDataToType<long long int>(const CSVData& data){
    return ConvertCSVDataToType<long long int,CONV_LONG_LONG>(data);
}

template<>
VECTOR_MAT(unsigned long long int) CSVDataToType<unsigned long long int>(const CSVData& data){
    return ConvertCSVDataToType<unsigned long long int,CONV_LONG_LONG>(data);
}

//Write the csv data to a stream
template<typename vector_mat_type>
std::ostream& operator<<(std::ostream& os, const VECTOR_MAT(vector_mat_type)& data){
    return WriteVecToStream(os,data,'\n');
}

template<typename T>
class CSVMatrix{
public:
    typedef T value_type;
    typedef std::vector<value_type> value_vector;
    typedef std::vector< value_vector > data_type;
    typedef typename data_type::iterator iterator;

protected:
    data_type data;

public:
    value_type MAX_VALUE;

    CSVMatrix(){
        MAX_VALUE = std::numeric_limits<value_type>::max();
    }

    CSVMatrix(data_type& startingData):data(startingData.begin(), startingData.end()){}

    unsigned int size(){    return data.size(); }
    iterator begin(){    return data.begin();    }
    iterator end(){    return data.end();    }

    void appendRow(const value_vector& newRow){
        data.push_back(newRow);
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

    value_vector& operator[](unsigned int indx){
        return data[indx];
    }

    friend std::ostream& operator << (std::ostream& os, const CSVMatrix& mat){
        return os <<  mat.data;
    }

    void operator = (const data_type& dat){
        data = dat;
    }
};


#endif // CSVPARSER_H
