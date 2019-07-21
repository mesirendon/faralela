#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>

int** createMatrix(int N){
    int** M = (int **)malloc(N * sizeof(int *));
    int i; 
    for (i=0; i<N; i++) 
        M[i] = (int *)malloc(N * sizeof(int));
    return M;
}

void readMatrix(char* filename, int** M, int N){
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

void printMatrix(int** M, int N){
    // print matrix for testing
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

void multiplyMatrix(int** A, int** B, int** C, int N, int ini, int fin){ 
    int i, j, k; 
    for (i = ini; i < fin; i++) { 
        for (j = 0; j < N; j++) { 
            C[i][j] = 0; 
            for (k = 0; k < N; k++) 
                C[i][j] += A[i][k]*B[k][j]; 
        } 
    } 
}

void writeMatrix(char *filename, int** C, int N){
    FILE *fp;
    int i,j;
    fp=fopen(filename,"w+");
    for(i=0 ; i<N; i++){
        for(j=0; j<N; j++){
            fprintf(fp,",%d ",C[i][j]);
        }
        fprintf(fp,"\n%d",i+1);
    }
    fclose(fp);
}

int main(int argc, char **argv){
    // Arguments
    if ( argc !=  5){
        printf("usage: ./matrixMult MatA.csv MatB.csv N H\n");
        return -1;
    }
    char* fileA = argv[1];
    char* fileB = argv[2];
    int N = atoi(argv[3]);
    int H = atoi(argv[4]);

    // Create, read & print matrix A
    int** A = createMatrix(N);
    readMatrix(fileA, A, N);
    printMatrix(A, N);

    // Create, read & print matrix B
    int** B = createMatrix(N);
    readMatrix(fileB, B, N);
    printMatrix(B, N);
    
    // Create C, obtain & print marix C
    int** C = createMatrix(N);

    #pragma omp parallel num_threads(H)
    {
        int tn = omp_get_thread_num();
        int ini = (int)(N/H)*(tn);
        int fin = (int)(N/H)+ini;
        multiplyMatrix(A, B, C, N, ini, fin);
    }

    printMatrix(C, N);

    // Write the matrix
    writeMatrix("result.csv", C, N);
    free(A);
    free(B);
    free(C);
    
    return 0;
}