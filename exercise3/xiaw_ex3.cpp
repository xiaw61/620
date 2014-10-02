#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
int main(int argc, char *argv[]) {
    const std::string Letters = 
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // Anonymous/first-class/lambda function to generate a random letter
    std::ofstream outFile("argv[2]");
    auto rndChar = [&Letters]{ return Letters[rand() % Letters.size()]; };
    // Assume: argv[1] = number of letters to generate
    // argv[2] = path to output file.
    std::generate_n(outFile,argv[1],rndChar);
    // Complete program to generate argv[1] number of English letters
    // (using rndChar function) and write them to file specified in
    // argv[2]. Tip: see generate_n algorithm.
    
    return 0;
}
