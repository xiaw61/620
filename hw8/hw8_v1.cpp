// copyright xiaw
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
void func(std::ifstream& dataFile, std::ofstream& outFile, char a, char b) {
    auto fun = [&](char c){ return c == a?b:c;};
    std::istream_iterator<char> data(dataFile >> std::noskipws), eof;
    std::ostream_iterator<char> out(outFile);
    std::transform(data, eof, out, fun);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cout << "Specify necessary command-line arguments.\n";
        return 5;
    }

    std::ifstream dataFile(argv[1]);
    std::ofstream outFile(argv[4]);
    if (dataFile.good()) {
        func(dataFile, outFile, argv[2][0], argv[3][0]);
    } else {
        std::cout << "Unable to read file: " << argv[1] << std::endl;
        return 1;
    }

    return 0;
}
