#!/bin/bash
# Usage : ./analyse_perfs_mpi.sh <nb_exec> <executable> [args...]
# Exécute le programme MPI avec np = 1,2,4,8 et calcule Speedup + Karp–Flatt + Amdahl

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <nb_exec> <executable> [args...]"
  exit 1
fi

N=$1; EXEC=$2; shift 2; ARGS="$@"
PROCS=(1 2 4 6)   # nombre de processus MPI à tester

# --- Fonction pour calculer le temps moyen ---
time_avg() {
  local p=$1 total=0
  for ((i=1;i<=N;i++)); do
    start=$(date +%s.%N)
    mpirun -np $p $EXEC $ARGS > /dev/null 2>&1
    end=$(date +%s.%N)
    t=$(awk -v s=$start -v e=$end 'BEGIN{print e-s}')
    total=$(awk -v a=$total -v b=$t 'BEGIN{print a+b}')
  done
  awk -v t=$total -v n=$N 'BEGIN{print t/n}'
}

# --- Temps séquentiel (1 processus) ---
T1=$(time_avg 1)
echo "Temps séquentiel (np=1): $T1 s"
echo "-----------------------------------------"

# --- Boucle sur 2,4,8 processus ---
for p in "${PROCS[@]}"; do
  Tp=$(time_avg $p)
  S=$(awk -v ts=$T1 -v tp=$Tp 'BEGIN{print ts/tp}')
  if [ "$p" -eq 1 ]; then
    e=0; Sm=1
  else
    e=$(awk -v S=$S -v p=$p 'BEGIN{print (1/S-1/p)/(1-1/p)}')
    Sm=$(awk -v f=$e 'BEGIN{if(f>0)print 1/f;else print "∞"}')
  fi
  printf "np=%-2d  TempsMoyen=%.4fs  Accélération=%.3f  Karp-Flatt=%.5f  Smax(Amdahl)=%.2f\n" \
         $p $Tp $S $e $Sm
done
