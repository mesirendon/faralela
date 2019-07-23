#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

char* createFilename(int N, char* fileBeging, char* fileEnding){
    char* filename = malloc(strlen(fileBeging) + sizeof(char)*sizeof(int)*4 + strlen(fileEnding) + 1);
    sprintf(filename, "%s%d%s", fileBeging, N, fileEnding);
    return filename;
}

unsigned int* createMatrix(int N){
    return (unsigned int*)malloc(N * N * sizeof(unsigned int*));
}

void readMatrix(char* filename, unsigned int* M, int N){
    FILE *fstream = fopen(filename, "r");
    if(fstream == NULL){
        printf("\n file opening failed ");
        return;
    }
    char *record,*line;
    char buffer[10000];
    int i = 0;
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL){
        j = 0;
        record = strtok(line,",");
        while(record != NULL){
            M[i * N + j++] = atoi(record);
            record = strtok(NULL,",");
        }
        i++;
    }
}

void printMatrix(unsigned int* M, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
            printf("%d ",M[i * N + j]);
        printf("\n");
    }
    printf("\n");
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
    int number_of_processes, process, root = 0;
    // Arguments
    if ( argc !=  5){
        printf("usage: ./matrixMult MatA.csv MatB.csv N H\n");
        return -1;
    }
    char* fileA = argv[1];
    char* fileB = argv[2];
    int N = atoi(argv[3]);
    int H = atoi(argv[4]);

    unsigned int* matrixB = createMatrix(N);
    unsigned int* matrixC = createMatrix(N);
    readMatrix(fileB, matrixB, N);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process);

    unsigned int chunk = N * N / number_of_processes;

    // Send tags    
    int tagA = 1;
    int tagC = 3;

    // Starting time
    int r = clock_gettime(CLOCK_MONOTONIC, &tstart);

    if(process == root) {

        // Prepare Matrices A and B
        unsigned int* matrixA = createMatrix(N);
        readMatrix(fileA, matrixA, N);

        // Send chunks of matrix A
        int offset = chunk;
        for (int destination = 0; destination < number_of_processes; destination++)
        {
            MPI_Send(A[offset], chunk, MPI_INT, destination, tagA, MPI_COMM_WORLD);
            offset += chunk;
        }
        
        // Receive chunks of matrix C
        offset = chunk;
        for (int source = 0; source < number_of_processes; source++)
        {
            MPI_Recv(C[offset], chunk, MPI_INT, source, tagC, MPI_COMM_WORLD, &status);
			offset += chunk;	
        }

        free(matrixA);
        
    }

    unsigned int* minA = (unsigned int*)malloc(chunk * sizeof(unsigned int*));
    unsigned int* minC = (unsigned int*)malloc(chunk * sizeof(unsigned int*));

    MPI_Recv(minA[0] , chunk, MPI_INT, root, tagA, MPI_COMM_WORLD, &status);

    for (int i = 0; i < (N / number_of_processes); i++)
        for (int j = 0; j < N; j++)
        {			
            minC[i * N + j] = 0 ;	
            for (int k = 0; k < N; k++)
                minC[i * N + j] += minA[i * N + k] * B[k * N + j];
        }
    
    MPI_Send(minC[0], chunk, MPI_INT, root, tagC, MPI_COMM_WORLD);

    // Juntar pedazos de resultado

    if(process == root) {
        // Save matrix c into file
        char* fileC = createFilename(N, "output/", "C.csv");
        writeMatrix(fileC, matrixC, N);
        free(matrixB);
        free(matrixC);
    }

    MPI_Finalize();

    // Ending time
    r = clock_gettime(CLOCK_MONOTONIC, &tend);
    double delta = time_spec_seconds(&tend) - time_spec_seconds(&tstart);
    printf("%d,%d,%.8f\n", N, H, delta);

    return 0;
}