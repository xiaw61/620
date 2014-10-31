// Copyright 2014 raodm@miamiOH.edu

#include <stdexcept>
#include <string>
#include <iostream>
#include "ClCmdQueue.h"
#include "BitmapImage.h"

// A couple of forward declarations to keep compiler happy
int getDevType(const std::string& device) throw(std::runtime_error);
BitmapImage clVfunc(ClCmdQueue& queue, cl::KernelFunctor& vfunc,
                    BitmapImage& srcImg, std::vector<float> fliter)
    throw(cl::Error);

// A simple tester for testing operation of toggling characters via OpenCL

std::vector<float> readFliter(std::string fileName) {
    std::ifstream file(fileName);
    std::vector<float> ret;
    float temp = 0;
    while(file >> temp) {
        ret.push_back(temp);
    }
    return ret;
}
int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cout << "Specify [CPU|GPU|ACC] <InputBMP> <OutputBMP>\n";
        return 1;
    }
    // Load source image from the given file
    BitmapImage srcImg;
    std::vector<float> fliter = readFliter(argv[4]);
    srcImg.load(argv[2]);
    // Create the OpenCL command queue for a given device
    ClCmdQueue queue(getDevType(argv[1]));
    // Create a kernel and functor using OpenCL kernel code from a file.
    cl::Kernel kernel = queue.loadKernel("hw6.cl", "hw6");
    cl::NDRange globalRange(srcImg.getWidth(), srcImg.getHeight());
    cl::KernelFunctor vfunc(kernel, queue.getQueue(), cl::NullRange,
                            globalRange, cl::NullRange);
    BitmapImage outImg = clVfunc(queue, vfunc, srcImg,fliter);
    outImg.write(argv[3]);
    return 0;
}

BitmapImage
clVfunc(ClCmdQueue& queue, cl::KernelFunctor& vfunc,
        BitmapImage& srcImg, std::vector<float> fliter) throw (cl::Error) {
    BitmapImage outImg = srcImg;
    // Create the input, output image, and filter buffers!
    cl::Image2D srcBuf = queue.makeImage(srcImg.getRow(0), srcImg.getWidth(),
                                         srcImg.getHeight(), queue.ROFlags);
    cl::Image2D outBuf = queue.makeImage(outImg.getRow(0), outImg.getWidth(),
                                         outImg.getHeight(), queue.WOFlags);

    cl::Buffer fBuf = queue.makeBuffer(&fliter[0],fliter.size()*sizeof(float),
                                       queue.ROFlags);
    // Setup sampler for source image to control sampler
    cl::Sampler sampler(queue.getContext(), CL_FALSE, CL_ADDRESS_CLAMP,
                        CL_FILTER_NEAREST);
    // Now use the OpenCl kernel to perform convolution
    vfunc(srcBuf, outBuf, srcImg.getWidth(), srcImg.getHeight(), sampler,
          fBuf);
    // Copy result image back from the device to host.
    queue.enqueueMapImage(outBuf);
    // Return the resulting image back to the caller.
    return outImg;
}

int getDevType(const std::string& device) throw(std::runtime_error) {
    const std::string DevTypes = "CPUGPUACC";
    switch (DevTypes.find(device)) {
    case 0: return CL_DEVICE_TYPE_CPU;
    case 3: return CL_DEVICE_TYPE_GPU;
    case 6: return CL_DEVICE_TYPE_ACCELERATOR;
    }
    throw std::runtime_error("Invalid device type specified");
}

// End of source code
