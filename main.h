#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <iostream>
#include "CSVData.h"
#include <limits>
#include <fstream>
#include <cmath>

enum MathOperation{
    MATH_ADD,
    MATH_SUB,
    MATH_MUL,
    MATH_DIV
};

template<typename T>
std::vector<T> applyOperationForEachElement(MathOperation op, std::vector<T> vec, const std::vector<T>& value){
    for(unsigned int i = 0; i < vec.size(); ++i){
        switch(op){
            case MATH_ADD:
                vec[i] += value[i];
                break;
            case MATH_SUB:
                vec[i] -= value[i];
                break;
            case MATH_MUL:
                vec[i] *= value[i];
                break;
            case MATH_DIV:
                vec[i] /= value[i];
                break;
        }
    }
    return vec;
}

template<typename T>
std::vector< std::vector<T> > applyOperationToEachRow(MathOperation op, std::vector< std::vector<T> > mat, const std::vector<T>& value){
    for(unsigned int i = 0; i < mat.size(); ++i){
        mat[i] = applyOperationForEachElement(op, mat[i], value);
    }
    return mat;
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

#endif // MAIN_H_INCLUDED
