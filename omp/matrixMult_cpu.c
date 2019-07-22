#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

char* createFilename(int N, char* fileBeging, char* fileEnding){
    char* filename = malloc(strlen(fileBeging) + sizeof(char)*sizeof(int)*4 + strlen(fileEnding) + 1);
    sprintf(filename, "%s%d%s", fileBeging, N, fileEnding);
    return filename;
}

unsigned long long** createMatrix(int N){
    unsigned long long** M = (unsigned long long **)malloc(N * sizeof(unsigned long long *));
    int i; 
    for (i=0; i<N; i++) 
        M[i] = (unsigned long long *)malloc(N * sizeof(unsigned long long));
    return M;
}

void readMatrix(char* filename, unsigned long long** M, int N){
    FILE *fstream = fopen(filename, "r");
    if(fstream == NULL){
        printf("\n file opening failed ");
        return;
    }
    char *record,*line;
    char buffer[1024];
    int i=0,j=0;
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL){
        j = 0;
        record = strtok(line,",");
        while(record != NULL){
            M[i][j++] = atoi(record);
            record = strtok(NULL,",");
        }
        ++i;
    }
}

void printMatrix(unsigned long long** M, int N){
    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%lld ",M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void ones(unsigned long long** M, int N){
    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            M[i][j] = 1;
        }
    }
}

void multiplyMatrixOpenMP(unsigned long long** A, unsigned long long** B, unsigned long long** C, int N){ 
    int i, j, k;
    #pragma omp parallel for
    for (i = 0; i < N; i++)
    {
        #pragma omp parallel for
        for (j = 0; j < N; j++)
        { 
            C[i][j] = 0;
            #pragma omp parallel for 
            for (k = 0; k < N; k++) 
                C[i][j] += A[i][k]*B[k][j]; 
        } 
    }
}

void multiplyMatrix(unsigned long long** A, unsigned long long** B, unsigned long long** C, int N){ 
    int i, j, k;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        { 
            C[i][j] = 0; 
            for (k = 0; k < N; k++) 
                C[i][j] += A[i][k]*B[k][j]; 
        } 
    }
}

void writeMatrix(char *filename, unsigned long long** C, int N){
    FILE *fp;
    int i,j;
    fp=fopen(filename,"w+");
    for(i=0 ; i<N; i++){
        for(j=0; j<N-1; j++){
            fprintf(fp,"%lld,",C[i][j]);
        }
        fprintf(fp,"%lld",C[i][j]);
        fprintf(fp,"\n");
    }
    fclose(fp);
}

double time_spec_seconds(struct timespec* ts) {
    return (double) ts->tv_sec + (double) ts->tv_nsec * 1.0e-9;
}

int main(int argc, char **argv){
    struct timespec tstart = {0,0}, tend = {0,0};
    // Arguments
    if ( argc !=  5){
        printf("usage: ./matrixMult MatA.csv MatB.csv N H\n");
        return -1;
    }
    char* fileA = argv[1];
    char* fileB = argv[2];
    int N = atoi(argv[3]);
    int H = atoi(argv[4]);

    // Create and read matrix A
    unsigned long long** matrixA = createMatrix(N);
    //readMatrix(fileA, matrixA, N);
    //ones(matrixA, N);
    //printMatrix(A, N);

    // Create and read matrix B
    unsigned long long** matrixB = createMatrix(N);
    //readMatrix(fileB, matrixB, N);
    //ones(matrixB, N);
    //printMatrix(B, N);
    
    // Create C, obtain & print marix C
    unsigned long long** matrixC = createMatrix(N);

    // Parallel computing
    int r = clock_gettime(CLOCK_MONOTONIC, &tstart);
    if(H == 1){
        multiplyMatrix(matrixA, matrixB, matrixC, N);
    }
    else {
        multiplyMatrixOpenMP(matrixA, matrixB, matrixC, N);
    }
    r = clock_gettime(CLOCK_MONOTONIC, &tend);
    double delta = time_spec_seconds(&tend) - time_spec_seconds(&tstart);
    printf("%d\t%d\t%.8f\n", N, H, delta);
    
    // Write matrix C on csv
    char* fileC = createFilename(N, "output/", "C.csv");
    writeMatrix(fileC, matrixC, N);

    //Free memory
    free(matrixA);
    free(matrixB);
    free(matrixC);
    
    return 0;
}