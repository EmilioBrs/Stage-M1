#!/bin/bash
#
#SBATCH --job-name=ILC_l2_v02_segmentationspatiale
#SBATCH --output=output1.out
#SBATCH --error=output1.err
#
#SBATCH --ntasks=1
#SBATCH --time=2-00:00:00 
#SBATCH --mem-per-cpu=1G
#SBATCH --partition=long
# mail-type=BEGIN, END, FAIL, REQUEUE, ALL, STAGE_OUT, TIME_LIMIT_90
#SBATCH --mail-type=ALL
#SBATCH --mail-user=e.bourasseau@ip2i.in2p3.fr

