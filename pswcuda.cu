/****************************************************************************
  Similar to factorise_3_0 but solves the problem with 4 threads using a 
  block method for search space partitioning. It is included here to 
  accompany a CUDA version of the program.

  Compile with:

   nvcc -o pswcuda pswcuda.cu
  
  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>

__device__ int is_a_match(char *password){

	char pw1[]="BI1100";
	char pw2[]="KR2200";
	char pw3[]="RA4540";
	char pw4[]="PU6080";

	
	char *p1 = password;
	char *p2 = password;
	char *p3 = password;
	char *p4 = password;

	
	char *w1 = pw1;
	char *w2 = pw2;
	char *w3 = pw3;
	char *w4 = pw4;


	while(*p1 == *w1){


	if(*p1 == '\0'){

	printf("Password found : %s\n", pw1);
	return 1;

	}
	p1++;
	w1++;
}


while(*p2 == *w2){


	if(*p2 == '\0'){

	printf("Password found : %s\n", pw2);
	return 1;

	}
	p2++;
	w2++;
}


while(*p3 == *w3){


	if(*p3 == '\0'){

	printf("Password found : %s\n", pw3);
	return 1;

	}
	p3++;
	w3++;
}


while(*p4 == *w4){


	if(*p4 == '\0'){

	printf("Password found : %s\n", pw4);
	return 1;

	}
	p4++;
	w4++;
}
return 0;

}


__global__ void kernel(){

char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'S', 'T','U', 'V', 'W', 'X', 'Y', 'Z'};


char numbers[10] = {'0','1','2','3','4','5','6','7','8','9'};

char password[7];
password[6] = '\0';

int a, b, c, d;

for(a=0;a<10;a++){

	for(b=0;b<10;b++){

		for(c=0;c<10;c++){

			for(d=0;d<10;d++){

				password[0] = alphabet[blockIdx.x];
				password[1] = alphabet[threadIdx.x];
				password[2] = numbers[a];
				password[3] = numbers[b];
				password[4] = numbers[c];
				password[5] = numbers[d];

				if(is_a_match(password)){

					printf("Password found: %s\n", password);
			}
		}
			


	}



}

}




}



int time_difference(struct timespec *start, struct timespec *finish, 
                              long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char *argv[]) {
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
	 kernel <<<26, 26>>>();
	cudaDeviceSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", 
    time_elapsed, (time_elapsed/1.0e9)); 

  return 0;
}
