#!/bin/bash
#
#SBATCH --job-name=ILC_l2_v02_spatialsegmentation
#SBATCH --output=output1.out
#SBATCH --error=output1.err
#
#SBATCH --ntasks=1
#SBATCH --time=2-00:00:00 
#SBATCH --mem-per-cpu=2G
#SBATCH --partition=long
# mail-type=BEGIN, END, FAIL, REQUEUE, ALL, STAGE_OUT, TIME_LIMIT_90
#SBATCH --mail-type=ALL
#SBATCH --mail-user=e.bourasseau@ip2i.in2p3.fr

. /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03/init_ilcsoft.sh
export MARLIN_DLL=/home/ilc/ebourasseau/StageProcessor/lib/libStageProcessor.so

echo "" > StageProcessor/results/datafiles/superminimums.txt

cd ~/StageProcessor/build/
make
make install
cd ~
 
list=(30 40 50 60 70 80 91 100 110 120 130 140 150 160 170 180 190 200 220 240 260 280 300 350 360 400 450 500 550 600)

for elt in "${list[@]}"
do
	./marlin_nrj.sh $elt > ~/outputs/file_"$elt".out &
done

wait