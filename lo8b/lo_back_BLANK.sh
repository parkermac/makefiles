#!/bin/bash

## Job Name
#SBATCH --job-name=LiveOcean

## Resources
## Nodes
#SBATCH --nodes=$N_NODES$
## Tasks per node (Slurm assumes you want to run 28 tasks per node unless explicitly told otherwise)
#SBATCH --ntasks-per-node=$CORES_PER_NODE$

## Walltime 
#SBATCH --time=05:00:00

## Memory per node
#SBATCH --mem=128G

## Specify the working directory for this job
#SBATCH --chdir=/gscratch/macc/parker/LiveOcean_roms/makefiles/$EX_NAME$

module load icc_17-impi_2017
module load netcdf_fortran+c_4.4.1.1-icc_17

echo $EXE_PATH$

mpirun ./oceanM $EXE_PATH$liveocean.in > $EXE_PATH$log.txt

