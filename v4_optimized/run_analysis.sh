#!/bin/bash

# Çıkış klasörü
OUTPUT_DIR="profiling_results"
mkdir -p "$OUTPUT_DIR"

# Derleme
echo "[1] Derleme başlatılıyor..."
g++ -O2 -lpthread -pg -o v4_exec_O2 main.cpp Regressor.cpp
g++ -O3 -lpthread -pg -o v4_exec_O3 main.cpp Regressor.cpp
echo "Derleme tamamlandı."

# Gprof analizleri
echo "[3] Gprof analizleri..."
./v4_exec_O2 > /dev/null
gprof v4_exec_O2 gmon.out > "$OUTPUT_DIR/gprof_O2.txt"
rm -f gmon.out

./v4_exec_O3 > /dev/null
gprof v4_exec_O3 gmon.out > "$OUTPUT_DIR/gprof_O3.txt"
rm -f gmon.out
echo "Gprof tamamlandı."

# mpstat ölçümü
echo "[4] mpstat ölçümü..."
./v4_exec_O3 > /dev/null &  # Arka planda çalışsın
v4_PID=$!
mpstat -P ALL 1 5 > "$OUTPUT_DIR/mpstat.txt"
wait $v4_PID
echo "mpstat tamamlandı."

# Zaman ölçümü
echo "[5] Time ölçümü..."
echo "--- O2 ---" > "$OUTPUT_DIR/time.txt"
{ time ./v4_exec_O2 > /dev/null; } 2>> "$OUTPUT_DIR/time.txt"

echo "--- O3 ---" >> "$OUTPUT_DIR/time.txt"
{ time ./v4_exec_O3 > /dev/null; } 2>> "$OUTPUT_DIR/time.txt"

echo "Tüm işlemler tamamlandı. Sonuçlar '$OUTPUT_DIR/' klasörüne kaydedildi."
