#include "PositionDataIO.h"

void PositionData::pushPosition(const ptype& p){
    data.push_back(p);
}

void PositionData::pushPosition(const unsigned short& x, const unsigned short& y){
    pushPosition(std::make_pair(x,y));
}

PositionDataIO::PositionDataIO(){
}

PositionDataIO::~PositionDataIO(){
}

void PositionDataIO::readPositionFromFile(const std::ifstream& file){

}
