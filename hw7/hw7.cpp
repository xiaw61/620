#include <stdexcept>
#include <string>
#include "ClCmdQueue.h"
#include <vector>
#include <string>
std::vector<char> readData(const std::string& fileName)
    throw(std::runtime_error){
    std::vector<char> ret;
    std::ifstream file(fileName);
    if (!file.good()) {
        throw std::runtime_error("Error opening input file.");
    }
    std::string temp;
    while(file >> temp) {
        for(char ch: temp) {
            ret.push_back(ch);
        }
        ret.push_back(NULL);

    }
    return ret;    
}
std::vector<int> indexCharList(std::vector<char> charList) {
    std::vector<int> ret;
    ret.push_back(0);
    int index = 0;
    for(char ch:charList) {
        index++;
        if(ch == NULL)ret.push_back(index);
    }
    return ret;
}
void  putStringRight(std::vector<int> a, std::vector<int> b,
                                 std::vector<char> & charList,char *ret) {

    for(decltype(a.size()) i = 0; i < a.size(); i++) {
        int m = a[i];
        int n = b[i];
        while(charList[m] != NULL) {
            ret[n++] =  charList[m++];
        }
        ret[n] = NULL;
    }

}
void wirte2File(std::vector<char> charList,std::vector<int> index,
                std::string fileName) {
    
    std::ofstream outFile(fileName);
    for(int i = 0; i < index.size();i++){
       
        std::string str( &(charList[index[i]]) );
        outFile <<str<<'\n';       
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "Specify [CPU|GPU|ACC] <File> <CharToSearch>\n"
                  << "Example: " << argv[0] << " input.txt a\n";
        return 1;
    }
    std::vector<char> charList = readData(argv[2]);
    std::vector<int> index = indexCharList(charList);
    wirte2File(charList,index,argv[3]);

}
