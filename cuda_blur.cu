#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"



__global__ void blur_image(unsigned char * gpu_imageOuput, unsigned char * gpu_imageInput,int width,int height){

    int counter=1;

    int idx = blockDim.x * blockIdx.x + threadIdx.x;

    
    int i=blockIdx.x;
    int j=threadIdx.x;


    int t_r=0;
	int t_g=0;
	int t_b=0;
    int t_a=0;


    if(i+1 && j-1){

        // int pos= idx/2-2;

        int pos=blockDim.x * (blockIdx.x+1) + threadIdx.x-1;
        int pixel = pos*4;
        
        t_r=gpu_imageInput[idx*4];
        t_g=gpu_imageInput[idx*4+1];
        t_b=gpu_imageInput[idx*4+2];
        t_a=gpu_imageInput[idx*4+3];

        t_r += gpu_imageInput[pixel];
        t_g += gpu_imageInput[1+pixel];
        t_b += gpu_imageInput[2+pixel];
        t_a += gpu_imageInput[3+pixel];
        
        counter++;



    }

    if(j+1){

        // int pos= idx/2-2;

        int pos=blockDim.x * (blockIdx.x) + threadIdx.x+1;

        int pixel = pos*4;

        t_r=gpu_imageInput[idx*4];
        t_g=gpu_imageInput[idx*4+1];
        t_b=gpu_imageInput[idx*4+2];
        t_a=gpu_imageInput[idx*4+3];

        t_r += gpu_imageInput[pixel];
        t_g += gpu_imageInput[1+pixel];
        t_b += gpu_imageInput[2+pixel];
        t_a += gpu_imageInput[3+pixel];

        counter++;
    }

    if(i+1 && j+1){

        // int pos= idx/2+1;

        int pos=blockDim.x * (blockIdx.x+1) + threadIdx.x+1;


        int pixel = pos*4;

        t_r=gpu_imageInput[idx*4];
        t_g=gpu_imageInput[idx*4+1];
        t_b=gpu_imageInput[idx*4+2];
        t_a=gpu_imageInput[idx*4+3];

        t_r += gpu_imageInput[pixel];
        t_g += gpu_imageInput[1+pixel];
        t_b += gpu_imageInput[2+pixel];
        t_a += gpu_imageInput[3+pixel];

        counter++;


    }

    if(i+1){
        // int pos= idx+1;

        int pos=blockDim.x * (blockIdx.x+1) + threadIdx.x;

        int pixel = pos*4;

        t_r=gpu_imageInput[idx*4];
        t_g=gpu_imageInput[idx*4+1];
        t_b=gpu_imageInput[idx*4+2];
        t_a=gpu_imageInput[idx*4+3];

        t_r += gpu_imageInput[pixel];
        t_g += gpu_imageInput[1+pixel];
        t_b += gpu_imageInput[2+pixel];
        t_a += gpu_imageInput[3+pixel];

        counter++;



    }

    if(j-1){

        // int pos= idx*2-2;
        int pos=blockDim.x * (blockIdx.x) + threadIdx.x-1;

        int pixel = pos*4;

        t_r=gpu_imageInput[idx*4];
        t_g=gpu_imageInput[idx*4+1];
        t_b=gpu_imageInput[idx*4+2];
        t_a=gpu_imageInput[idx*4+3];

        t_r += gpu_imageInput[pixel];
        t_g += gpu_imageInput[1+pixel];
        t_b += gpu_imageInput[2+pixel];
        t_a += gpu_imageInput[3+pixel];

        counter++;




    }

    if(i-1){

        // int pos= idx-1;
        int pos=blockDim.x * (blockIdx.x-1) + threadIdx.x;

        int pixel = pos*4;

        t_r=gpu_imageInput[idx*4];
        t_g=gpu_imageInput[idx*4+1];
        t_b=gpu_imageInput[idx*4+2];
        t_a=gpu_imageInput[idx*4+3];

        t_r += gpu_imageInput[pixel];
        t_g += gpu_imageInput[1+pixel];
        t_b += gpu_imageInput[2+pixel];
        t_a += gpu_imageInput[3+pixel];

        counter++;


    }
    
    int current_pixel=idx*4;

    gpu_imageOuput[current_pixel]=t_r/counter;
    gpu_imageOuput[1+current_pixel]=t_g/counter;
    gpu_imageOuput[2+current_pixel]=t_b/counter;
    gpu_imageOuput[3+current_pixel]=gpu_imageInput[3+current_pixel];


}



int main(int argc, char **argv){

	unsigned int error;
	unsigned int encError;
	unsigned char* image;
	unsigned int width;
	unsigned int height;
	const char* filename = "pic.png";
	const char* newFileName = "blur.png";

	error = lodepng_decode32_file(&image, &width, &height, filename);
	if(error){
		printf("error %u: %s\n", error, lodepng_error_text(error));
	}

	const int ARRAY_SIZE = width*height*4;
	const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned char);

	unsigned char host_imageInput[ARRAY_SIZE * 4];
	unsigned char host_imageOutput[ARRAY_SIZE * 4];

	for (int i = 0; i < ARRAY_SIZE; i++) {
		host_imageInput[i] = image[i];
	}

	// declare GPU memory pointers
	unsigned char * d_in;
	unsigned char * d_out;

	// allocate GPU memory
	cudaMalloc((void**) &d_in, ARRAY_BYTES);
	cudaMalloc((void**) &d_out, ARRAY_BYTES);

	cudaMemcpy(d_in, host_imageInput, ARRAY_BYTES, cudaMemcpyHostToDevice);

	// launch the kernel
	blur_image<<<height, width>>>(d_out, d_in,width,height);


	// copy back the result array to the CPU
	cudaMemcpy(host_imageOutput, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);
	
	encError = lodepng_encode32_file(newFileName, host_imageOutput, width, height);
	if(encError){
		printf("error %u: %s\n", error, lodepng_error_text(encError));
	}

	//free(image);
	//free(host_imageInput);
	cudaFree(d_in);
	cudaFree(d_out);

	return 0;
}
