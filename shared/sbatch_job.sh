#!/bin/bash

#SBATCH -t 00:05:00
#SBATCH -n 1
#SBATCH --job-name=dependentJob
#SBATCH --mail-type=end
#SBATCH --mail-user=pmacc@uw.edu

echo "Dependent job ran at: " `date`