#include "main.h"
#include <bitset>

#define LN() cout << endl
#define LOG(x) cout << x << endl

using namespace std;
using namespace Arithmetic;

template<typename T>
std::ostream& bitsToStream(std::ostream& os, const T& val){
    for(unsigned int i = 8*sizeof(T); i > 0; --i){
        os << getBit(val,(i-1));
    }
    return os;
}

template<typename T>
void printBits(const T& s){
    bitsToStream(cout, s);
}

int main(){
    cout << "Read Data From File" << endl;
    VECTOR_MAT(unsigned char) combinedData = readCombinedDataFromFile();
    cout << "Shuffle Data" << endl;
    shuffleVector(combinedData);
    cout << "Data Size: " << combinedData.size() << endl;
    cout << "Split Data" << endl;
    unsigned int splitAbout = Lerp<unsigned int>(0,combinedData.size(),0.1);
    cout << "Split About: " << splitAbout << endl;
    pair< VECTOR_MAT(unsigned char), VECTOR_MAT(unsigned char) > splitData = splitVector(combinedData,splitAbout);
    cout << "New Sizes " << splitData.first.size() << " + " << splitData.second.size() << " = " << (splitData.first.size()+splitData.second.size()) << endl;

    ProbabilityModel model(true);
    Encoder<ProbabilityModel> encoder(&model);

    cout << "Count Training Values" << endl;
    for(unsigned int i = 0; i < splitData.second.size(); ++i){
        model.countValue(splitData.second[i][0]);
    }

    cout << "Get Counts" << endl;
    vector<unsigned int> counts;
    for(unsigned int i = 0; i < model.maxSize() ; ++i){
        ProbabilityModel::count_pair tmpc = model.getCount((unsigned char)i);
        counts.push_back(tmpc.second-tmpc.first);
    }

    cout << "Largest Count: ";
    pair<unsigned int, unsigned int> maxCount = getMax(counts);
    //cout << maxCount.first << "=>" << maxCount.second << endl;

    cout << "Encode Values" << endl;
    for(unsigned int i = 0; i < splitData.first.size(); ++i){
        encoder.encodeSymbol(splitData.first[i][0]);
    }

    BitStream encoded_stream = encoder.endEncoding();

    cout << "Original Number Of Bits:\t" << splitData.first.size() * 8 << endl;
    cout << "Encoded Number Of Bits: \t" << encoded_stream.numberOfBits() << endl;
    unsigned int bitSavings = (splitData.first.size() * 8) - encoded_stream.numberOfBits();
    double percentSavings = 100*bitSavings/(splitData.first.size() * 8.0);
    double savingsPerMessage = bitSavings / (double)splitData.first.size();
    cout << "Bit Savings:\t\t\t" << bitSavings << "\t" << percentSavings << "%\t" << savingsPerMessage << endl;

    cout << endl << "Calculate Savings" << endl;
    unsigned int updatesPerSecond = 33;
    double savingsPerSecond = updatesPerSecond * savingsPerMessage;
    cout << "Savings Per Update:        \t" << savingsPerMessage << endl;
    cout << "Update Messages Per Second:\t" << updatesPerSecond << endl;
    cout << "Savings/player/second:     \t" << savingsPerSecond << " bps" <<endl;

    cout << endl << "Second Level" << endl;
    cout << "Extract Only Max Representation, where first values was " << maxCount.first << endl;

    vector<unsigned char> secondLevelData;
    for(unsigned int i = 0; i < combinedData.size(); ++i){
        if(combinedData[i][0] == maxCount.first)
            secondLevelData.push_back(combinedData[i][1]);
    }

    cout << "Found " << secondLevelData.size() << " data points" << endl;
    ProbabilityModel secondModel(true);
    Encoder<ProbabilityModel> secondEncoder(&secondModel);

    for(unsigned int i = 0; i < secondLevelData.size(); ++i){
        secondModel.countValue(secondLevelData[i]);
    }

    cout << "Encode Second Level" << endl;
    for(unsigned int i = 0; i < secondLevelData.size(); ++i){
        secondEncoder.encodeSymbol(secondLevelData[i]);
    }

    BitStream second_encoded_stream = secondEncoder.endEncoding();
    cout << "Second Original number Of Bits:\t" << secondLevelData.size()*8 << endl;
    cout << "Second Encoded number of Bits: \t" << second_encoded_stream.numberOfBits() << endl;
    unsigned int secondBitSavings = (secondLevelData.size()*8) - second_encoded_stream.numberOfBits();
    double secondPercentSavings = 100*secondBitSavings/(secondLevelData.size()*8.0);
    double secondSavingsPerMessage = secondBitSavings / (double)secondLevelData.size();
    cout << "Second Bit Savings:\t\t" << secondBitSavings << "\t" << secondPercentSavings << "%\t" << secondSavingsPerMessage << endl;

    cout << endl << "Calculate Secondary Savings" << endl;
    double secondSavingsPerSecond = updatesPerSecond * secondSavingsPerMessage;
    cout << "Savings Per Update:        \t" << secondSavingsPerMessage << endl;
    cout << "Update Messages Per Second:\t" << updatesPerSecond << endl;
    cout << "Savings/player/second:     \t" << secondSavingsPerSecond << " bps" << endl;

    cout << endl << "Calculate Total Savings" << endl;
    cout << "Savings Per Update:        \t" << savingsPerMessage + secondSavingsPerMessage << endl;
    cout << "Update Messages Per Second:\t" << updatesPerSecond << endl;
    cout << "Savings/player/second:     \t" << savingsPerSecond+secondSavingsPerSecond << " bps" << endl;


    cout << endl << "DONE...";
    cin.ignore();
    return 0;
}
