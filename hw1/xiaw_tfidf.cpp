// copyright xiaw
#include <ctype.h>
#include <regex>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <cmath>
#include <map>
#include <algorithm>
typedef std::unordered_map<std::string, double> stringAndCountMap;

bool isQualify(std::string * str) {
    for (auto ch = (*str).begin(); ch != (*str).end(); ch++) {
        if (std::isalpha(*ch) == false && *ch != '-' ) return false;
        if (std::isalpha(*ch)) {
            *ch = tolower(*ch);
        }
    }
    if ((*str).size() >= 4) {
        size_t b = 0;
        size_t e = 3;
        if ((*str).substr(b, e) == "http")return false;
    }
    return true;
}

stringAndCountMap getTermStringFromFile(stringAndCountMap  * resultMap,
                                        std::string fileName) {
    std::ifstream dataFile(fileName);


    stringAndCountMap::iterator got;
    std::string temp;
    while (dataFile >> temp) {
         if (isQualify(&temp)) {
             got = (*resultMap).find(temp);
             if (got ==(*resultMap).end())
                 (*resultMap).insert(std::make_pair(temp, 1));
            else
                got->second = (got->second)+1;
             }
    }

    return *resultMap;
}
void covertToLowerCase(std::string str) {
    for (auto it = str.begin(); it != str.end(); it++) {
        *it = tolower(*it);
    }
}
int  caculateCountInFile(stringAndCountMap * resultMap, std::string fileName) {
    std::ifstream dataFile(fileName);
    std::string temp;
    int result = 0;
    stringAndCountMap::iterator got;
    while (dataFile >> temp) {
        result++;
        covertToLowerCase(temp);
        got = (*resultMap).find(temp);
        if (got != (*resultMap).end())got->second = (got->second)+1;
    }
    return result;
}
void caculateSig(stringAndCountMap * resultMap,
                 const stringAndCountMap & termMap, int num) {
    for (auto it = (*resultMap).begin(); it != (*resultMap).end(); it++) {
        auto got = termMap.find(it->first);
        it->second = got->second* log10(num/(it->second));
    }
}
int caculateFWD(stringAndCountMap * resultMap,
                const stringAndCountMap & termMap, std::string fileName) {
    std::ifstream dataFile(fileName);
    std::cout << "Loading corpus using files listed in "<< fileName <<std::endl;
    std::string file;
    int numOfFile = 0;
    int numOfWord = 0;
    for (auto it = termMap.begin(); it != termMap.end(); it++) {
        (*resultMap).insert(std::make_pair(it->first, 0));
    }
    while (getline(dataFile, file)) {
        numOfFile++;
        numOfWord += caculateCountInFile(resultMap, file);
    }
    caculateSig(resultMap, termMap, numOfFile);
    std::cout << "Loaded corpus of " << numOfWord << " words from " <<
        numOfFile << "file(s)" << std::endl;
    std::cout << "---------[Starting analysis]------------" << std::endl;
    return numOfFile;
}

void caculateNTopTerm(stringAndCountMap termAndCount, int num,
                      std::multimap< double, std::string> * mutiMap) {
    std::multimap< double, std::string> tempMutiMap;
    for (auto it = termAndCount.begin(); it != termAndCount.cend(); ++it) {
        tempMutiMap.insert(make_pair(it->second, it->first));
    }
    int count =num;
        for (auto it = tempMutiMap.rbegin(); it != tempMutiMap.rend(); it++) {
            (*mutiMap).insert(make_pair(it->first, it->second));
        count--;
        if (count == 0)break;
    }
}
void printSigTermInFile(std::multimap< double, std::string> mutiMap,
                        std::string fileName) {
    std::ifstream dataFile(fileName);
    std::string str;
    int num = 0;
    while (getline(dataFile, str)) {
        std::string tempStr(str);
        num++;
        int count = 0;
        std::transform(str.begin(), str.end(), str.begin(), tolower);
        for (auto it = mutiMap.rbegin(); it!= mutiMap.rend(); it++) {
            std::size_t found = str.find(it->second);
            while (found != std::string::npos) {
                found =str.find(it->second, found+1);
                count++;
            }
        }
        if (count >= 2)std::cout << num << "[" << count << "]   "
                                << tempStr << std::endl;
    }
}
int main(int argc, char *argv[]) {
    if (argc != 4)std::cout << " argument is not enough!";
    stringAndCountMap searchFileString;
    getTermStringFromFile(&searchFileString, argv[2]);
    stringAndCountMap termAndCount;
    caculateFWD(&termAndCount, searchFileString, argv[1]);
    std::multimap< double, std::string> mutiMap;
    caculateNTopTerm(termAndCount, atoi(argv[3]), & mutiMap);
    for (auto it = mutiMap.rbegin(); it != mutiMap.rend(); it++) {
        std::cout <<it->second <<":"<< it->first <<std::endl;
    }
    printSigTermInFile(mutiMap, argv[2]);
    return 0;
}
