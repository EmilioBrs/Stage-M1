#!/bin/bash

NRJ=$1

#. /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03/init_ilcsoft.sh
#export MARLIN_DLL=/home/ilc/ebourasseau/StageProcessor/lib/libStageProcessor.so


cd ~/StageProcessor/build/
make
make install
cd ~/StageProcessor/steer
Marlin steer_${NRJ}.xml
mv result_${NRJ}.root ../results/rootfiles/  
