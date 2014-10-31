__kernel
void hw6(__read_only image2d_t  srcImage, __write_only image2d_t  outImage, 
           int width, int height, sampler_t sampler,__global float* fliter) {

 
    const int2 coords = {get_global_id(0), get_global_id(1)};
    //const int2 flipCoords = {coords.x, height - coords.y - 1};
    
    int x =0;
    int y =0;
    int z = 0;

    for(int i = -3; i < 4 ; i++) {
	for(int j = -3; j < 4; j++) {
		if( (coords.x+i >=0) && (coords.x+i <width) &&
                     (coords.y+j >=0) && (coords.y+j <height) ) {
			int2 temp = {coords.x+i, coords.y+j};
			uint4 pixel = read_imageui(srcImage, sampler, temp);
			x += pixel.x * fliter[(j+3)*7 + i+3 ];
			y += pixel.y * fliter[(j+3)*7 + i+3 ];
			z += pixel.z * fliter[(j+3)*7 + i+3 ];

		}	
	}
    }

	uint4 p = {x,y,z,0};
   write_imageui(outImage, coords, p);


}