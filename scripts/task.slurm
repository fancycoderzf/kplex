#!/bin/bash
# task.slurm
#SBATCH --job-name=clique
#SBATCH --mail-type=end
#SBATCH --mail-user=yi.zhou@univ-angers.fr
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=1:0:0
#SBATCH --mem-per-cpu=2G
#SBATCH -p amd-opteron-4184

# Dynamic Linking Library
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/yi.zhou/Gurobi_Env/gurobi811/linux64/lib

# your bash script
# you could give the control to python or whatever you like

python3 scripts/run.py ${SLURM_ARRAY_TASK_ID} $1