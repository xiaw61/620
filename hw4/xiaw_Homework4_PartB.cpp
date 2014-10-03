// copyright xiaw
// Compiler command-line:
//     make SRC=exercise7.cpp LIBS=-lOpenCL
//
// Alternative (not preferred):
//     g++ -g -Wall -std=c++11 exercise7.cpp -o execise7 -lOpenCL

// Enable OpenCL C++ exceptions
#define __CL_ENABLE_EXCEPTIONS
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include "CL/cl.hpp"
#include "./vector_operations.h"


// Implement the following add method (add suitable helper methods as
// you see fit) to use an OpenCL kernel to add each entry in the the
// two vectors, that is: result[i] = vec1[i] + vec2[i]
const std::string kernelSrc=
    "#pragma OPENCL EXTENSION cl_khr_fp64 : enable \n"
    "typedef float Type;"
    "typedef std::vector<Type> NumList;"
    "__kernel NumList \n"
    "vadd(__global float * a, __global float* b,"
    "__global float* c) { \n"
    "size_t i = get_global_id(0); // Get index to operate on \n"
    "c[i] = a[i] + b[i]; \n"
    "}"
    "__kernel NumList \n"
    "vmin(__global float * a, __global float* b,"
    "__global float* c) { \n"
    "size_t i = get_global_id(0); // Get index to operate on \n"
    "c[i] = a[i] - b[i]; \n"
    "}"
    "__kernel NumList \n"
    "vd(__global float * a, __global float* b,"
    "__global float* c) { \n"
    "size_t i = get_global_id(0); // Get index to operate on \n"
    "c[i] = a[i] / b[i]; \n"
    "}"
    "__kernel NumList \n"
    "vm(__global float * a, __global float* b,"
    "__global float* c) { \n"
    "size_t i = get_global_id(0); // Get index to operate on \n"
    "c[i] = a[i] * b[i]; \n"
    "}";
cl::Kernel createKernel1(cl::Context context, cl::Device device) {
    cl::Program::Sources sources(1, std::make_pair(kernelSrc.c_str(), 0));
    cl::Program program(context, sources);
    // Build Open CL program for a given device
    try {
        program.build({ device });
    } catch (cl::Error error) {
        std::cout << "Build log:" << std::endl
                  << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
                  << std::endl;
        throw error;
    }
    // Create a kernel from a program
    cl::Kernel kernel(program, "vad");
    return kernel;
}
cl::Kernel createKernel2(cl::Context context, cl::Device device) {
    cl::Program::Sources sources(1, std::make_pair(kernelSrc.c_str(), 0));
    cl::Program program(context, sources);
    // Build Open CL program for a given device
    try {
        program.build({ device });
    } catch (cl::Error error) {
        std::cout << "Build log:" << std::endl
                  << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
                  << std::endl;
        throw error;
    }
    // Create a kernel from a program
    cl::Kernel kernel(program, "vmin");
    return kernel;
}
cl::Kernel createKernel3(cl::Context context, cl::Device device) {
    cl::Program::Sources sources(1, std::make_pair(kernelSrc.c_str(), 0));
    cl::Program program(context, sources);
    // Build Open CL program for a given device
    try {
        program.build({ device });
    } catch (cl::Error error) {
        std::cout << "Build log:" << std::endl
                  << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
                  << std::endl;
        throw error;
    }
    // Create a kernel from a program
    cl::Kernel kernel(program, "vm");
    return kernel;
}
cl::Kernel createKernel4(cl::Context context, cl::Device device) {
    cl::Program::Sources sources(1, std::make_pair(kernelSrc.c_str(), 0));
    cl::Program program(context, sources);
    // Build Open CL program for a given device
    try {
        program.build({ device });
    } catch (cl::Error error) {
        std::cout << "Build log:" << std::endl
                  << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
                  << std::endl;
        throw error;
    }
    // Create a kernel from a program
    cl::Kernel kernel(program, "vd");
    return kernel;
}
NumList operator+(const NumList& nl1, const NumList& nl2) {
    std::vector<cl::Platform> platformList;
    cl::Platform::get(&platformList);
    std::vector<cl::Device> deviceList;
    platformList[0].getDevices(CL_DEVICE_TYPE_ALL, &deviceList);
    cl::Context context = cl::Context(deviceList);
    cl::Device device = deviceList[0];
    cl::Kernel kernel =createKernel1(context, device);
    cl::CommandQueue queue = cl::CommandQueue(context, device);
    const int ROFlags = CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR;
    const int bytesSize = nl1.size() * sizeof(Type);
    cl::Buffer vec1Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl1[0]));
    cl::Buffer vec2Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl2[0]));
    std::vector<Type> nl3(nl1.size());
    cl::Buffer vec3Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl3[0]));
    kernel.setArg(0, vec1Buf);
    kernel.setArg(1, vec2Buf);
    kernel.setArg(2, vec3Buf);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                               cl::NDRange(nl1.size()), cl::NullRange);
    queue.enqueueMapBuffer(vec3Buf, CL_TRUE, CL_MAP_READ, 0, bytesSize);
    return nl3;
}
NumList operator-(const NumList& nl1, const NumList& nl2) {
    std::vector<cl::Platform> platformList;
    cl::Platform::get(&platformList);
    std::vector<cl::Device> deviceList;
    platformList[0].getDevices(CL_DEVICE_TYPE_ALL, &deviceList);
    cl::Context context = cl::Context(deviceList);
    cl::Device device = deviceList[0];
    cl::Kernel kernel =createKernel2(context, device);
    cl::CommandQueue queue = cl::CommandQueue(context, device);
    const int ROFlags = CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR;
    const int bytesSize = nl1.size() * sizeof(Type);
    cl::Buffer vec1Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl1[0]));
    cl::Buffer vec2Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl2[0]));
    std::vector<Type> nl3(nl1.size());
    cl::Buffer vec3Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl3[0]));
    kernel.setArg(0, vec1Buf);
    kernel.setArg(1, vec2Buf);
    kernel.setArg(2, vec3Buf);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                               cl::NDRange(nl1.size()), cl::NullRange);
    queue.enqueueMapBuffer(vec3Buf, CL_TRUE, CL_MAP_READ, 0, bytesSize);
    return nl3;
}
NumList operator*(const NumList& nl1, const NumList& nl2) {
    std::vector<cl::Platform> platformList;
    cl::Platform::get(&platformList);
    std::vector<cl::Device> deviceList;
    platformList[0].getDevices(CL_DEVICE_TYPE_ALL, &deviceList);
    cl::Context context = cl::Context(deviceList);
    cl::Device device = deviceList[0];
    cl::Kernel kernel =createKernel3(context, device);
    cl::CommandQueue queue = cl::CommandQueue(context, device);
    const int ROFlags = CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR;
    const int bytesSize = nl1.size() * sizeof(Type);
    cl::Buffer vec1Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl1[0]));

    cl::Buffer vec2Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl2[0]));
    std::vector<Type> nl3(nl1.size());
    cl::Buffer vec3Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl3[0]));
    kernel.setArg(0, vec1Buf);
    kernel.setArg(1, vec2Buf);
    kernel.setArg(2, vec3Buf);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                               cl::NDRange(nl1.size()), cl::NullRange);
    queue.enqueueMapBuffer(vec3Buf, CL_TRUE, CL_MAP_READ, 0, bytesSize);
    return nl3;
}
NumList operator/(const NumList& nl1, const NumList& nl2) {
    std::vector<cl::Platform> platformList;
    cl::Platform::get(&platformList);
    std::vector<cl::Device> deviceList;
    platformList[0].getDevices(CL_DEVICE_TYPE_ALL, &deviceList);
    cl::Context context = cl::Context(deviceList);
    cl::Device device = deviceList[0];
    cl::Kernel kernel =createKernel4(context, device);
    cl::CommandQueue queue = cl::CommandQueue(context, device);
    const int ROFlags = CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR;
    const int bytesSize = nl1.size() * sizeof(Type);
    cl::Buffer vec1Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl1[0]));
    cl::Buffer vec2Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl2[0]));
    std::vector<Type> nl3(nl1.size());
    cl::Buffer vec3Buf = cl::Buffer(context, ROFlags, bytesSize
                                    , const_cast<Type *> (&nl3[0]));
    kernel.setArg(0, vec1Buf);
    kernel.setArg(1, vec2Buf);
    kernel.setArg(2, vec3Buf);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                               cl::NDRange(nl1.size()), cl::NullRange);
    queue.enqueueMapBuffer(vec3Buf, CL_TRUE, CL_MAP_READ, 0, bytesSize);
    return nl3;
}
int main() {
}
