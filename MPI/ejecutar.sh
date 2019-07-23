#!/bin/bash

echo '╔═════════════════════════════════════╗'
echo '║ Multiplicacion matrices con OpenMPI ║'
echo '╚═════════════════════════════════════╝'
echo '╔═════════════════════════════════════╗'
echo '║ Preparando y compilando...          ║'
echo '╚═════════════════════════════════════╝'
rm -rf logs input output && mkdir logs input output
gcc genRandMatrix.c -o genRandMatrix && ./genRandMatrix
mpicc matrix_mul_mpi.c -o matrix_mul_mpi -lm

multiply() {
    mpirun -np $process_number --hostfile mpi-hosts ./matrix_mul_mpi ./input/${N}A.csv ./input/${N}B.csv $N | tee -a ./logs/matrixMult_cpu.csv
}

echo '╔═════════════════════════════════════╗'
echo '║ Ejecutando y calculando tiempos...  ║'
echo '╚═════════════════════════════════════╝'
echo "N,H,T"
echo '═══════════════════════════════════════'
echo "N,H,T" > logs/matrixMult_cpu.csv

echo '╔═════════════════════════════════════╗'
echo '║            Matrices 8x8             ║'
echo '╚═════════════════════════════════════╝'
process_number=1; N=8; multiply
process_number=2; N=8; multiply
process_number=3; N=8; multiply
process_number=4; N=8; multiply

echo '╔═════════════════════════════════════╗'
echo '║           Matrices 16x16            ║'
echo '╚═════════════════════════════════════╝'
process_number=1; N=16; multiply
process_number=2; N=16; multiply
process_number=3; N=16; multiply
process_number=4; N=16; multiply

echo '╔═════════════════════════════════════╗'
echo '║           Matrices 32x32            ║'
echo '╚═════════════════════════════════════╝'
process_number=1; N=32; multiply
process_number=2; N=32; multiply
process_number=3; N=32; multiply
process_number=4; N=32; multiply

echo '╔═════════════════════════════════════╗'
echo '║           Matrices 64x64            ║'
echo '╚═════════════════════════════════════╝'
process_number=1; N=64; multiply
process_number=2; N=64; multiply
process_number=3; N=64; multiply
process_number=4; N=64; multiply

echo '╔═════════════════════════════════════╗'
echo '║          Matrices 128x128           ║'
echo '╚═════════════════════════════════════╝'
process_number=1; N=128; multiply
process_number=2; N=128; multiply
process_number=3; N=128; multiply
process_number=4; N=128; multiply

echo '╔═════════════════════════════════════╗'
echo '║          Matrices 256x256           ║'
echo '╚═════════════════════════════════════╝'
process_number=1; N=256; multiply
process_number=2; N=256; multiply
process_number=3; N=256; multiply
process_number=4; N=256; multiply

echo '╔═════════════════════════════════════╗'
echo '║          Matrices 512x512           ║'
echo '╚═════════════════════════════════════╝'
process_number=1; N=512; multiply
process_number=2; N=512; multiply
process_number=3; N=512; multiply
process_number=4; N=512; multiply

echo '╔═════════════════════════════════════╗'
echo '║         Matrices 1024x1024          ║'
echo '╚═════════════════════════════════════╝'
process_number=1; N=1024; multiply
process_number=2; N=1024; multiply
process_number=3; N=1024; multiply
process_number=4; N=1024; multiply