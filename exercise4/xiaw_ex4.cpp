
// copyright xiaw
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    std::istream_iterator<char> is(dataFile >> std::noskipws), efo;
    std::string strChar(is , efo);
    for ( int i = 2; i < argc; i++ ) {
        char low = argv[i][0];
        char hi = std::toupper(low);
        auto tx = [=](char c) {
            return (c == low ? hi : c == hi? low:c);
        };
        std::transform(strChar.begin(), strChar.end(), strChar.begin(), tx);
    }
    std::cout<< strChar;
    return 1;
}
