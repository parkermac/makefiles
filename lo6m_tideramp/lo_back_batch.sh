#!/bin/bash

## Job Name
#SBATCH --job-name=LiveOcean

## Resources
## Nodes
#SBATCH --nodes=7
## Tasks per node (Slurm assumes you want to run 28 tasks per node unless explicitly told otherwise)
#SBATCH --ntasks-per-node=28

## Walltime 
#SBATCH --time=1000:00

## Memory per node
#SBATCH --mem=128G

## Specify the working directory for this job
#SBATCH --workdir=/gscratch/macc/parker/LiveOcean_roms/makefiles/lo6m_tideramp

module load icc_17-impi_2017
module load netcdf_fortran+c_4.4.1.1-icc_17


echo /gscratch/macc/parker/LiveOcean_roms/output/cas3_v0_lo6m/f2013.01.01

mpirun ./oceanM /gscratch/macc/parker/LiveOcean_roms/output/cas3_v0_lo6m/f2013.01.01/liveocean.in > /gscratch/macc/parker/LiveOcean_roms/output/cas3_v0_lo6m/f2013.01.01/log.txt

