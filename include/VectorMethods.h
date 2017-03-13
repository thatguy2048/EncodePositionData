#ifndef VECTORMETHODS_H_INCLUDED
#define VECTORMETHODS_H_INCLUDED

#include <vector>
#include <ostream>
#include <sstream>

enum MathOperation{
    MATH_ADD,
    MATH_SUB,
    MATH_MUL,
    MATH_DIV
};

//perform a math operation on each element of a vector
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
//perform a math operation on each row of a multidimensional vector
template<typename T>
std::vector< std::vector<T> > applyOperationToEachRow(MathOperation op, std::vector< std::vector<T> > mat, const std::vector<T>& value){
    for(unsigned int i = 0; i < mat.size(); ++i){
        mat[i] = applyOperationForEachElement(op, mat[i], value);
    }
    return mat;
}

//write a vector to a stream, separating the values by delim
template<typename T>
std::ostream& WriteVecToStream(std::ostream& os, const std::vector<T>& vec, const char& delim){
    for(unsigned int i = 0; i < vec.size(); ++i){
        if(i>0) os << delim;
        os << vec[i];
    }
    return os;
}

//write a vector to a stream, separating the values by commas
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec){
    return WriteVecToStream(os,vec,',');
}

//split a string by a char delimiter into a vector of strings
std::vector<std::string> SplitString(const std::string& str, const char& delim){
    std::vector<std::string> output;
    std::stringstream ss(str);
    std::string s;
    while(std::getline(ss, s, delim)){
        output.push_back(s);
    }
    return output;
}

template<typename T>
std::pair< std::vector<T>, std::vector<T> > splitVector(const std::vector<T>& vec, unsigned int splitIndx){
    return std::make_pair(std::vector<T>(vec.begin(),vec.begin()+splitIndx),std::vector<T>(vec.begin()+splitIndx,vec.end()));
}

template<typename T>
std::pair<unsigned int, T> getMax(const std::vector<T>& vec){
    unsigned int max_indx = 0;
    for(unsigned int i = 1; i < vec.size(); ++i){
        if(vec[max_indx] < vec[i]){
            max_indx = i;
        }
    }
    return std::make_pair(max_indx,vec[max_indx]);
}


#endif // VECTORMETHODS_H_INCLUDED
