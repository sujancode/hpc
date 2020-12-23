#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_SIZE 1024

typedef struct parameters{
    int x,y;
}args;


int matrix1[MAX_SIZE][MAX_SIZE];
int matrix2[MAX_SIZE][MAX_SIZE];
int result[MAX_SIZE][MAX_SIZE];

int max_thread;


void *multiply(void* arg){
    args* parm=arg;

    for(int a=0;a<MAX_SIZE;a++){
        result[parm->x][parm->y]+=matrix1[parm->x][a]*matrix2[a][parm->y];
       
    }
}

void init_matrix(){
    for(int i=0;i<MAX_SIZE;i++){
        for(int j=0;j<MAX_SIZE;j++){
            matrix1[i][j]=rand()%10;
            matrix2[i][j]=rand()%10;
            result[i][j]=0;
        }
    }
}

void start_thread(){

    int x=0,y=0;

    while(x<MAX_SIZE){
         y=0;

        while(y<MAX_SIZE){
            pthread_t threads[max_thread];
            args parm[max_thread]; 

            for(int i=0;i<max_thread;i++){
                
                if(y>=MAX_SIZE){
                    break;
                }

                    parm[i].x=x;
                    parm[i].y=y;
                    pthread_create(&threads[i], NULL, multiply, (void *) &parm[i]);
                
                    pthread_join(threads[i],NULL);

                     y++;

            }


        

            
        }
        x++;

    }



}

int main(){
    printf("Initilizing Matrix\n");
    init_matrix();
    printf("Done\n");

    printf("Enter Threads To use:");
    scanf("%d",&max_thread);

    printf("\nMultiplication Started\n");
    printf("Matrix1:%ld\nMatrix2:%ld\n",sizeof(matrix1)/sizeof(matrix1[0]),sizeof(matrix2)/sizeof(matrix2[0]));
    printf("Threads:%d\n",max_thread);
    
    clock_t begin = clock();

    start_thread();    
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time Spend:%lf",time_spent);


}