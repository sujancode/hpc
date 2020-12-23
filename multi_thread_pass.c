#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>

int len_pass=10;

char *passwords[]={
"$6$AS$qYED/fL4D6y3V2etSYy0UNd.yU.8IAdqtHEhhI/UJhRJ0JB0kw5Q1ldzmKewzmCgSpOttqEN6.yUPy7u.1v2M1",
"$6$AS$xE0xX.IfE.wIAI1yKKKi981uMDZFivSId26S/PRmhpeRznzsT6sLKoU0SB.I.EP44lNYWLavm3gWkl4Hv46eo1",
"$6$AS$69u3dGDw8M3vbMtn5v8Mod71OVtQhoXCjYQtd3Zz0YumBmH3vpVHsZ0vtUdebSHdzSkeqeMux6SOO0IYut7WB1",
"$6$AS$.amVf4uzMS.dnlKExuQ8Gr9jTu8QIIM39W7H17zjfOyuLkhRWjG73MWeFo/rJUceD0UQ8et4KKhhuHQSpgeVQ."
"$6$AS$eqk/9wlOcIGDtYT5L7Ht9txRjoAcACXlfkaCQ6XDfRYgNe7wX/T5Ltgaop6cqzamuimNeFPLQHVd9u0UgDzzO/",
"$6$AS$oT.jTAyagWMebWIZyDiLK/go0jpvLOKgH/Rm6AFs6tatls1le1zWNDgrIL3CNHAtlS9Si4y7OdSuRtPcPdVXh/",
"$6$AS$HpLEy3pqTM1Ztl5QxLg2oKhOq/Ul9KfZ3gjJl/rB2.SzhEU0wyvZ5WMMJJlcgntD6JAGbYRjOIkAmTDSQrijK.",
"$6$AS$ecN6Q3sIvufJzW8HZJkqgvymiCcMPMLAGXX544RrNNwMYWHwDZYStnzxkY42xospvGBFcxxaLrnxEQwsk3bmf1",
"$6$AS$qq/mcpFA.NVl3VjyfbNIvSrKhvOMpRqpp./VWzUVDwv6GfqvGYrHiaZ/8KSHTayK5eXOdCeRIbRrK/rihBqa71",
"$6$AS$E8UmCAR4yStqBrp6QKx6Zd5t6Hy4Lgz21ihko0U21glDajoraJG71oQ0QxkIcRr2S6VOpY.KS5Ht2ievVCL65/"
};

void substr(char *dest, char *src, int start, int length){
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}


void *kernal_funtion_1(void *salt_and_encrypted){
    int i, j,l;
    char salt[7];
    char plain[7];
    char *enc;
    int count = 0;

    substr(salt, salt_and_encrypted, 0, 6);

    for(i='A'; i<='M'; i++){
        for(j='A'; j<='Z'; j++){
            for(l=0; l<=99; l++){
                sprintf(plain, "%c%c%02d", i,j,l);
                enc = (char *) crypt(plain, salt);
                count++;
                if(strcmp(salt_and_encrypted, enc) == 0){
                    printf("#%-8d%s %s\n", count, plain, enc);
                    return;
                }else{
                    printf("#%-8d%s %s\n", count, plain, enc);
                }
            }
        }
    }
    printf("%d solutions explored\n", count);
}

void *kernal_funtion_2(void *salt_and_encrypted){
    int i, j, l;
    char salt[7];
    char plain[7];
    char *enc;
    int count = 0;

    substr(salt, salt_and_encrypted, 0, 6);

    for(i='N'; i<='Z'; i++){
        for(j='A'; j<='Z'; j++){
            for(l=0; l<=99; l++){
                sprintf(plain, "%c%c%02d", i,j,l);
                enc = (char *) crypt(plain, salt);
                count++;
                if(strcmp(salt_and_encrypted, enc) == 0){
                    printf("#%-8d%s %s\n", count, plain, enc);
                    return;
                }else{
                    printf("#%-8d%s %s\n", count, plain, enc);
                }
            }
        }
    }
    printf("%d solutions explored\n", count);
}



int main(){

    double total_time=0.0;
    pthread_t thread1,thread2;
    
    for(int i=0;i<len_pass;i++){
        clock_t begin = clock();

        pthread_create(&thread1,NULL,kernal_funtion_1,passwords[i]);
        pthread_create(&thread2,NULL,kernal_funtion_2,passwords[i]);

        pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

        printf("Time Spend:%lf",time_spent);

        total_time+=time_spent;
    }

  printf("Average Mean Time:%lf\n",total_time/len_pass);
  return 0;
}