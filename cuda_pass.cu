#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
#include <crypt.h>


//cuda encrypter
__device__ char* CudaCrypt(char* rawPassword){

	char * newPassword = (char *) malloc(sizeof(char) * 11);

	newPassword[0] = rawPassword[0] + 2;
	newPassword[1] = rawPassword[0] - 2;
	newPassword[2] = rawPassword[0] + 1;
	newPassword[3] = rawPassword[1] + 3;
	newPassword[4] = rawPassword[1] - 3;
	newPassword[5] = rawPassword[1] - 1;
	newPassword[6] = rawPassword[2] + 2;
	newPassword[7] = rawPassword[2] - 2;
	newPassword[8] = rawPassword[3] + 4;
	newPassword[9] = rawPassword[3] - 4;
	newPassword[10] = '\0';

	for(int i =0; i<10; i++){
		if(i >= 0 && i < 6){ //checking all lower case letter limits
			if(newPassword[i] > 122){
				newPassword[i] = (newPassword[i] - 122) + 97;
			}else if(newPassword[i] < 97){
				newPassword[i] = (97 - newPassword[i]) + 97;
			}
		}else{ //checking number section
			if(newPassword[i] > 57){
				newPassword[i] = (newPassword[i] - 57) + 48;
			}else if(newPassword[i] < 48){
				newPassword[i] = (48 - newPassword[i]) + 48;
			}
		}
	}
	return newPassword;
}

__device__ bool cudaCmp(char* s1,char* s2){
    for(int i=0;i<sizeof(s1);i++){
        if(s1[i]!=s2[i])
            return false;
    }
    return true;

}

  //password checker
__device__ int check_pass(char* enc){
    int len_pass=10;


    char *passwords[]={
        "bwzdwy7323",
        "bwzdwy9523",
        "bwzdwy1662",
        "sorwqs7345",
        "gcfdwy4045",
        "bwzdwy7334",
        "bwzdwy7345",
        "zvydwy9562",
        "tpsdwy8453",
        "zvydwy4045"
    };
    
    for(int i=0;i<len_pass;i++){
        if(cudaCmp(passwords[i],enc))
            return 1;
        else
            return 0;
        
    }
    return 0;
}

//crack kernel
__global__ void crack_kernel(char *alphabet, char *number){
    char genRawPass[4];

    genRawPass[0]=alphabet[blockIdx.x];
    genRawPass[1]=alphabet[blockIdx.y];

    genRawPass[2]=number[threadIdx.x];
    genRawPass[3]=number[threadIdx.y];

    char *enc=CudaCrypt(genRawPass);
    if(check_pass(enc))
        printf("%s\n",genRawPass);
}

//main function
int main(int argc,char ** argv){
    char cpuAlphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char cpuNumbers[26] = {'0','1','2','3','4','5','6','7','8','9'};

    char * gpuAlphabet;
    cudaMalloc( (void**) &gpuAlphabet, sizeof(char) * 26); 
    cudaMemcpy(gpuAlphabet, cpuAlphabet, sizeof(char) * 26, cudaMemcpyHostToDevice);

    char * gpuNumbers;
    cudaMalloc( (void**) &gpuNumbers, sizeof(char) * 26); 
    cudaMemcpy(gpuNumbers, cpuNumbers, sizeof(char) * 26, cudaMemcpyHostToDevice);

    clock_t begin = clock();
    crack_kernel<<<dim3(26,26,1),dim3(10,10,1)>>>(gpuAlphabet,gpuNumbers);
    cudaDeviceSynchronize();
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time Spend:%lf\n",time_spent);
    return 0;
}