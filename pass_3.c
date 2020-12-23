#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>


int len_pass=10;

char *passwords[]={
"$6$AS$ph4N99PLqqZbP535KZ0Kh9u6Zz0fzxhicPYEpE/xYsZXkKMBhkRvDm32KM8vuG9rsSvzXQdMrkPskiVgqcwGf/",
"$6$AS$4TZ.TmRsRevT.jP3iDc/alPGJjOSmFmsq5vM6NLy2oOn7QErT4OJCPgNRxldG8I.kYXfFxteI6QXO425TZZx40",
"$6$AS$rtyi5erxjx1NL4wGO.chZuwBHRk2nhs7P3ct5IhTCrZ1j6BHzDSZ7QTh.59RdTCkL6GL9tlHSXyWn9Xnkb7pI0",
"$6$AS$6HWs8LYPGkUykWzIftlQYpAyEicFbgKjKNL7mmoBykID6wk.LrSwdxQaJGCZJFPG5VNXkkTvjcgtAt9Pc6V8t.",
"$6$AS$Zs2L5V5sZyVHqg8McM.IX6MebgdAbofO7xfV1XtbHo3UbUWVQJvyFpaxAV04N9ET/NPisP2PXzthGuKKyyD301",
"$6$AS$M1EWbnZv2l5iVGdnXV1x2S70PelsTUJLSDKDGpwLIuCoAmY8Kmz9hTuEHS2HjCoKaXcdHt13nOzSwq8LkIOo91",
"$6$AS$BRstio6yoZ1Az/wFYHvGMqbt0K9/kl5h6RUDtK0nLxw.Gvyj17ZAZADT5Jt3r5PumO8dYK2TL8Lc.P3g333Pt.",
"$6$AS$TWkaiMqnLHo1PWHJPNgwIeq.SmwM5fYCXa.y/UQEA.xm60HU3wkWr2c0ZxzCCuVyWDKJ0kOSnN904cN/eeeDb.",
"$6$AS$xvQvQJ/e.FJVVoi74ksSOn6v6h0M5I8ZOCouXztGKxKGGz7vC4BakD1dVay7NN7m39o2D/8Bdf1d1OIpZ.3oq1",
"$6$AS$ayTW5l8dk7YFP73CYcLZrg/ookFgVNFQtWNZHunakK5L2IZ3zsIQ5E4ft81rwa8C5cAsHeAJe.5dkDXqse4fI/"
};


void substr(char *dest, char *src, int start, int length){
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}

void crack(void *salt_and_encrypted){
    int i, j, k,l;
    char salt[7];
    char plain[7];
    char *enc;
    int count = 0;

    substr(salt, salt_and_encrypted, 0, 6);

    for(i='A'; i<='M'; i++){
        for(j='A'; j<='Z'; j++){
            for(k='A';k<='Z';k++){
                    for(l=0; l<=99; l++){
                        sprintf(plain, "%c%c%c%02d", i,j,k,l);
                        enc = (char *) crypt(plain, salt);
                        count++;
                        if(strcmp(salt_and_encrypted, enc) == 0){
                            printf("#%-8d%s %s\n", count, plain, enc);
                            return;
                        }
                        
                }
            }
        }
    }
    printf("%d solutions explored\n", count);
}

int main(int argc, char *argv[]){
  double total_time=0.0;

    for(int i=0;i<len_pass;i++){
        printf("\nIteration %d\nPassword%s\n",i,passwords[i]);
        clock_t begin = clock();
        crack(passwords[i]);		
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        total_time+=time_spent;
        printf("Time Spend:%lf",time_spent);
    }

    

  printf("Average Mean Time:%lf\n",total_time/len_pass);
  return 0;
}