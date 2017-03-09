#include "main.h"

#define LN() cout << endl
#define LOG(x) cout << x << endl
using namespace std;

int main(){
    string folder = "CS_Position_Information/";
    string files[] ={
        "CS_Dust_Position_00.csv",
        "CS_Dust_Position_01.csv",
        "CS_Dust_Position_02.csv",
        "CS_Dust_Position_03.csv"
    };

    fstream pos_stream;
    CSVData posData;

    for(unsigned int i = 0; i < 4; ++i){
        pos_stream.open(folder+files[i],fstream::in);
        pos_stream >> posData;
        pos_stream.close();
    }

    cout << "Number of points: " << posData.size() << endl;

    CSVDoubleData posDoubleData = CSVDataToDouble(posData);;
    CSVDoubleMatrix positionData;
    positionData.appendMoreData(posDoubleData);

    CSVDoubleMatrix::double_vector roundMax = positionData.max();
    CSVDoubleMatrix::double_vector roundMin = positionData.min();

    cout << "Position Data MAX: " << roundMax << endl;
    cout << "Position Data Min: " << roundMin << endl;
    cout << "Position Data Average: " << positionData.average() << endl;

    for(unsigned int i = 0; i < roundMax.size(); ++i){
        roundMax[i] = ceil(roundMax[i]/100)*100;
        roundMin[i] = floor(roundMin[i]/100)*100;
    }

    cout << "Round position Data MAX: " << roundMax << endl;
    cout << "Round position Data Min: " << roundMin << endl;

    LN();

    positionData.applyToEachRow(MATH_SUB, roundMin);
    positionData.applyToEachRow(MATH_DIV, applyOperationForEachElement(MATH_SUB,roundMax,roundMin));
    positionData.applyToEachRow(MATH_MUL, vector<double>(roundMax.size(),std::numeric_limits<short>::max()));

    cout << "Round position Data MAX: " << positionData.max() << endl;
    cout << "Round position Data Min: " << positionData.min() << endl;



    LN();
    cout << "DONE...";
    cin.ignore();
    return 0;
}
