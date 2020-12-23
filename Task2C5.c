#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>


int pass_len=5;
char *password[]={
    "$6$AS$NV5NfKX3IvVE7RPq3mQtZJbGYJ8psUaHV3RHSIIxLNpBZBMD8zjifrSK2NEi4Lc4Kib5o.r5nUiTPsbRjJi1t.",
    "$6$AS$NRkpxMLVR4wnnqv..GLho7HL.fqRnQVkdSRJX3lvx/GBmcbgf5gJkjZN6JqSYtO5W00hOdRhaFusjTnNaqAqL0",
    "$6$AS$jxQxboOeX.D0B4hh7wytcw1YS7x/EKETufL6L1z.tOsvaICLSF9C5weOsXYCRyjS2gyRCflY2Vw.U1PvK/Jwa.",
    "$6$AS$tk9ryvWfxK6IDpvhHTRsLZaGoH3DvKSWK/Xn39nUzBEqJ6ifBhIUkJJU8rPbosaGlOlKODTZpNPnbrK5QPL/j0",
    "$6$AS$WRIWDMTcLMZ99/R94/Oa2e18CETF5ky7PKd8v3s2h35jSGOGzt.gmjd7knTlm.RffnkVT3vDNmioV7WRVDMuC0"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void *kernel_function_2(void *str){

  char *salt_and_encrypted=str;
  int h, i, j;     
  char salt[7];    
  char plain[7];   
  char *enc;       

  substr(salt, salt_and_encrypted, 0, 6);

  for(h='N'; h<='Z'; h++){
    for(i='A'; i<='Z'; i++){
      for(j=0; j<=99; j++){
        sprintf(plain, "%c%c%02d", h, i, j); 
        enc = (char *) crypt(plain, salt);
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("%s %s\n", plain, enc);
        }
      }
    }
  }
  
}

void *kernel_function_1(void *str){
  char *salt_and_encrypted=str;
  int h, i, j;     
  char salt[7];    
  char plain[7];   
  char *enc;       

  substr(salt, salt_and_encrypted, 0, 6);

  for(h='A'; h<='M'; h++){
    for(i='A'; i<='Z'; i++){
      for(j=0; j<=99; j++){
        sprintf(plain, "%c%c%02d", h, i, j); 
        enc = (char *) crypt(plain, salt);
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("%s %s\n", plain, enc);
        }
      }
    }
  }
  
}

void main(){
    pthread_t thread1,thread2;

    for(int i=0;i<1;i++){
      pthread_create(&thread1,NULL,kernel_function_1,password[i]);
      pthread_create(&thread2,NULL,kernel_function_2,password[i]);

      pthread_join(thread1,NULL);
      pthread_join(thread2,NULL);

    }
}