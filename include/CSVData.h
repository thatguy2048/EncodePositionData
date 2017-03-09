#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>

typedef std::vector< std::vector<std::string> > CSVData;
typedef std::vector< std::vector<double> > CSVDoubleData;

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

#endif // CSVPARSER_H
