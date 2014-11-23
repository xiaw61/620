#include <string>
#include <iostream>
#include <algorithm>
#include <set>
#include <fstream>

std::set<std::string> dict;
void loaddic() {
    std::ifstream infile("english.txt");
    std::string line;
    while(std::getline(infile,line)) {
        dict.insert(line);     
    }
}
void func(char * str,int size) {
    //    std::string test(str);
    //    std::cout << test <<std::endl;
    std::sort(str,str+size);

    do {
        if(size >= 3) {
            std::string str3(str,3);
            if (dict.find(str3) != dict.end()) std::cout <<str3<<std::endl;
        }
        if(size >= 4) {
            std::string str4(str,4);
            if (dict.find(str4) != dict.end()) std::cout <<str4<<std::endl;
        }
        if(size >= 5) {
            std::string str5(str,5);
            if (dict.find(str5) != dict.end()) std::cout <<str5<<std::endl;
        }
        if(size >= 6) {
            std::string str6(str,6);
            if (dict.find(str6) != dict.end()) std::cout <<str6<<std::endl;
        }
        if(size >= 7) {
            std::string str7(str,7);
            if (dict.find(str7) != dict.end()) std::cout <<str7<<std::endl;
        }
    }while(std::next_permutation(str,str+size) );
    
}
int main() {
    loaddic();
    char * str = getenv("QUERY_STRING");
    std::string test(str);
    if(str != nullptr) func(str,test.size());
    else {
        std::string str;
        while( getline(std::cin,str) ) {
            func( const_cast<char*>(   str.c_str()),str.size());

        }

    }
}
