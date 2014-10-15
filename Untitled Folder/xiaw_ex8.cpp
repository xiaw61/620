#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <utility>


// Convert that the following toggle function to run on a OpenCL device
/*
void toggle(std::string& data, const char low) {
    const char up  = toupper(low);
    auto  toggle = [low,up](char c) {
        return (c == low ? up : c == up ? low : c);
    };
    std::transform(data.begin(), data.end(), data.begin(), toggle);
}
*/

cl::Program getProgram(std::vector<cl::Device>& deviceList, cl::Context& context,
                       const std::string& fileName) throw (cl::Error) {
    std::ifstream file("fileName");
    std::istream_iterator<char> is(file),efo;
    std::string srcString(is,efo);
    cl::Program::Sources sources(1,std::make_pair(srcString.c_str(),0));
    cl::Program program(context,sources);

    return program;

}

cl::Kernel getKernel(cl::Context& context,cl::CommandQueue& queue,
                     const std::string& fileName = "toggle.cl",
                     const std::string& entryFunc= "toggle") {

    

}

void toggle(std::string& data, const char low) {
    std::vector<cl::Platform> platformList;
    cl::Platform::get(&platformList);
    std::vector<cl::Device> deviceList;
    platformList[0].getDevices(CL_DEVICE_TYPE_ALL,&deviceList);
    cl::Context context = cl::Context(deviceList);
    cl::Device device = deviceList[0];
    
    cl::Program program = getProgram(deviceList,context);    
}

//-------------------------------------------------------------------
//     DO   NOT   MODIFY   CODE   BELOW   THIS   LINE
//-------------------------------------------------------------------
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Specify <File> <toggle_char> ...\n";
        return 1;
    }
    // Read the file.
    std::ifstream dataFile(argv[1]);
    dataFile >> std::noskipws;
    std::istream_iterator<char> dataIter(dataFile), eof;
    std::string data(dataIter, eof);
    // Replace specified charachters
    for(int curr = 2; (curr < argc); curr++) {
        toggle(data, argv[curr][0]);
    }
    // Print the modifed data.
    std::cout << data;
    return 0;
}
