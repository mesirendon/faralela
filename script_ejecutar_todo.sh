# Compile matrixMul_cpu
cd omp/
gcc -fopenmp matrixMult_cpu.c -o matrixMult_cpu

# Function for CPU
execute_matrixMult_cpu() {
./matrixMult_cpu ../input/${N}A.csv ../input/${N}B.csv ${N} ${H} | tee -a logs/matrixMult_cpu.csv
}

# Procesing images
echo '╔═════════════════════════╗'
echo '║     matrixMult_cpu      ║'
echo '╚═════════════════════════╝'
echo "N,H,T"
echo '═══════════════════════════'
echo "N,H,T" > logs/matrixMult_cpu.csv
N=8; H=1; execute_matrixMult_cpu
N=8; H=2; execute_matrixMult_cpu
N=16; H=1; execute_matrixMult_cpu
N=16; H=2; execute_matrixMult_cpu
N=32; H=1; execute_matrixMult_cpu
N=32; H=2; execute_matrixMult_cpu
N=64; H=1; execute_matrixMult_cpu
N=64; H=2; execute_matrixMult_cpu
N=128; H=1; execute_matrixMult_cpu
N=128; H=2; execute_matrixMult_cpu
N=256; H=1; execute_matrixMult_cpu
N=256; H=2; execute_matrixMult_cpu
N=512; H=1; execute_matrixMult_cpu
N=512; H=2; execute_matrixMult_cpu
N=1024; H=1; execute_matrixMult_cpu
N=1024; H=2; execute_matrixMult_cpu
echo '' # Break line