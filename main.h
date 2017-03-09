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

class CSVDoubleMatrix{
public:
    typedef std::vector<double> double_vector;

protected:
    bool b_max = {true}, b_min = {true}, b_avg = {true};
    double_vector _max, _min, _avg;

public:
    static double DBL_MAX;

    CSVDoubleData data;

    void appendMoreData(const CSVDoubleData& moreData){
        data.insert(data.end(), moreData.begin(), moreData.end());
        b_max = true;
        b_min = true;
    }

    double_vector max(){
        double_vector output;
        for(unsigned int i = 0; i< data.size(); ++i){
            while(output.size() < data[i].size())   output.push_back(0);
            for(unsigned int j = 0; j < data[i].size(); ++j){
                if(output[j] < data[i][j])  output[j] = data[i][j];
            }
        }
        return output;
    }

    double_vector min(){
        double_vector output;
        for(unsigned int i = 0; i< data.size(); ++i){
            while(output.size() < data[i].size())   output.push_back(DBL_MAX);
            for(unsigned int j = 0; j < data[i].size(); ++j){
                if(output[j] > data[i][j])  output[j] = data[i][j];
            }
        }
        return output;
    }

    double_vector average(){
        if(b_avg){
            _avg.clear();
            double numberOfPoints = data.size();
            for(unsigned int i = 0; i< data.size(); ++i){
                while(_avg.size() < data[i].size())   _avg.push_back(0);
                for(unsigned int j = 0; j < data[i].size(); ++j){
                    _avg[j] += data[i][j]/numberOfPoints;
                }
            }
            b_avg = false;
        }

        return _avg;
    }

    void applyToEachRow(MathOperation op, const double_vector& values){
        data = applyOperationToEachRow(op, data, values);
    }
};

double CSVDoubleMatrix::DBL_MAX = std::numeric_limits<double>::max();

#endif // MAIN_H_INCLUDED
