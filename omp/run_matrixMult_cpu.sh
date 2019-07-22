execute_matrixMult_cpu() {
./matrixMult_cpu input/$NA.csv input/$NB.csv $N $H | tee -a output/logs_matrixMult_cpu.csv
}

# Procesing images
echo '╔═════════════════════════╗'
echo '║     matrixMult_cpu      ║'
echo '╚═════════════════════════╝'
echo "N\tH\tT"
echo '═══════════════════════════'
echo "N\tH\tT" > output/logs_matrixMult_cpu.csv
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