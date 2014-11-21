// copyright xiaw
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#define SIZE 1024

void func(std::ifstream& dataFile, std::ofstream& outFile, char a, char b) {
    dataFile.seekg(0,dataFile.end);
    int size = dataFile.tellg();
    dataFile.seekg(0,dataFile.beg);
    auto fun = [&](char c){ return c == a?b:c;};
    std::istream_iterator<char> eof;
    std::ostream_iterator<char> out(outFile);
    char * buffer = new char [SIZE];
while((dataFile.read(buffer,SIZE))   ) {
        std::string temp(buffer);
        std::istringstream str(temp);

        std::istream_iterator<char> it(str >>std::noskipws);
        std::transform(it, eof, out, fun);
    }
 
// outFile<<"hahahahah\n";
 //delete buffer;
 // dataFile.read(buffer,(dataFile.tellg())%SIZE);
 // dataFile.read(buffer,30);
// outFile <<size<<"hahah";
 std::string temp(buffer,0,size%SIZE );
 std::istringstream str(temp);
  std::istream_iterator<char> it(str >>std::noskipws);
 std::transform(it, eof, out, fun);

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
