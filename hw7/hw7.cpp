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
    ret.pop_back();
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
void wirte2File(char * charList,int size,
                std::string fileName) {
    
    std::ofstream outFile(fileName);
    int start = 0;
    std::string str( &(charList[start]) );
    outFile <<str<<'\n';
    for(int i = 0; i < size;i++){
        if(charList[i] == NULL) {
            start = i;
            if( i != size-1) {
            std::string str( &(charList[start+1]) );
            outFile <<str<<'\n';
            }
        }
    }
}
std::vector<int> func(std::vector<char> charList,cl::KernelFunctor& sort,
                      std::vector<int> index, ClCmdQueue& queue) throw(cl::Error) {
    std::vector<int> ret(index.size());

    cl::Buffer charBuf = queue.makeBuffer(  const_cast<char *> (&charList[0]), charList.size(), queue.ROFlags);
    cl::Buffer indexBuf = queue.makeBuffer(  const_cast<int *>(&index[0]),index.size()*sizeof(int),queue.ROFlags);

    cl::Buffer retBuf = queue.makeBuffer( const_cast<int *> (&ret[0]),
                                           index.size()*sizeof(int), queue.RWFlags);
    int size = index.size();

    sort(charBuf,indexBuf,size,retBuf);
    queue.enqueueMapBuffer(retBuf, index.size()*sizeof(int));
    

    return ret;    
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
std::vector<int> sortedIndex(std::vector<char> charList,
                             std::vector<int> index, std::string unit) {
    std::vector<int> ret;
    int temp = getDevType(unit);
    ClCmdQueue queue(temp);

    cl::Kernel kernel   = queue.loadKernel("hw7.cl", "sort");
    //const int blockSize = 8192;
    int range = index.size();
    cl::KernelFunctor sort(kernel, queue.getQueue(), cl::NullRange,
                             cl::NDRange(range), cl::NullRange);
    ret = func(charList,sort,index,queue);

    return ret;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "Specify [CPU|GPU|ACC] <File> <CharToSearch>\n"
                  << "Example: " << argv[0] << " input.txt a\n";
        return 1;
    }
    std::vector<char> charList = readData(argv[2]);
    std::vector<int> index = indexCharList(charList);
    char ret[charList.size()];
    
    //    std::vector<int> index2={7,4,0};
    std::vector<int> index2=sortedIndex(charList,index,argv[1]);
    putStringRight(index,index2,charList,ret);
    wirte2File(ret,charList.size(),argv[3]);

}
