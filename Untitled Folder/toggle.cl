#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void
toggle(__global char *l,char c){

    size_t i = get_global_id(0);

    char up = c -'a'+'A';

    l[i]= l[i] == c ? up: l[i] == up ? c:l[i];
   
}