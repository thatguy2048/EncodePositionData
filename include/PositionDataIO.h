#ifndef POSITIONDATAIO_H
#define POSITIONDATAIO_H

#include <fstream>
#include <vector>

struct PositionData{
    typedef std::pair<unsigned short, unsigned short> ptype;
    std::vector<ptype> data;

    void pushPosition(const ptype& p);
    void pushPosition(const unsigned short& x, const unsigned short& y);
};

class PositionDataIO{
protected:
    PositionData data;

public:
    PositionDataIO();
    virtual ~PositionDataIO();

    PositionData* currentData(){    return &data;   }
    void readPositionFromFile(const std::ifstream& file);
};

#endif // POSITIONDATAIO_H
