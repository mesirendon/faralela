#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char* createFilename(int N, char* fileEnding){
    char* filename = malloc(sizeof(char)*sizeof(int)*4 + strlen(fileEnding) + 1);
    sprintf(filename, "%d%s", N, fileEnding);
    return filename;
}

int** createMatrix(int N){
    int** M = (int **)malloc(N * sizeof(int *));
    int i; 
    for (i=0; i<N; i++) 
        M[i] = (int *)malloc(N * sizeof(int));
    return M;
}

void printMatrix(int** M, int N){
    int i;
    int j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%d ",M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void randomMatrix(int** M, int N){
    int i;
    int j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            M[i][j] = (rand() % (100 - 0 + 1)) + 0;
        }
    }
}

void writeMatrix(char *filename, int** C, int N){
    FILE *fp;
    int i,j;
    fp=fopen(filename,"w+");
    for(i=0 ; i<N; i++){
        for(j=0; j<N-1; j++){
            fprintf(fp,"%d,",C[i][j]);
        }
        fprintf(fp,"%d",C[i][j]);
        fprintf(fp,"\n");
    }
    fclose(fp);
}

void randomMatrix2CSV(int N){
    char* fileA = createFilename(N, "A.csv");
    char* fileB = createFilename(N, "B.csv");
    int** matrixA = createMatrix(N);
    int** matrixB = createMatrix(N);
    randomMatrix(matrixA, N);
    randomMatrix(matrixB, N);
    writeMatrix(fileA, matrixA, N);
    writeMatrix(fileB, matrixB, N);
    free(matrixA);
    free(matrixB);
}

int main(int argc, char **argv){
    srand(time(0));
    int array[] = {8, 16, 32, 64, 128, 256, 512, 1024};
    int k, N;
    for(k=0; k<8; k++){
        N = array[k];
        printf("%d\n", N);
        randomMatrix2CSV(N);
    }
    return 0;
}