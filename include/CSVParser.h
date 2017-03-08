#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <sstream>
#include <vector>
#include <istream>

typedef std::vector< std::vector<std::string> > CSVData;

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

//parse the data in the stream, and add it to the provided data
//if data is null, create a new CSVData
//returns a pointer to the data
CSVData* CSVDataFromStream(CSVData* data, std::istream& in_str){
    if(data == NULL)    data = new CSVData();
    std::string line;
    while(std::getline(in_str,line,'\n')){
        data->push_back(SplitString(line,','));
    }
    return data;
}

#endif // CSVPARSER_H
