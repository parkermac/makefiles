"""
This creates a batch script for LiveOcean -- using a template version of 
the batch file replacing things like $whatever$ with actual values and paths.
-dsd- November 2015

"""

# get command line arguments
import argparse

parser = argparse.ArgumentParser()

# positional arguments
parser.add_argument('-xp', '--exe_path', type=str, help="output path to dot.in file")
parser.add_argument('-npn', '--np_num', type=int, help="number of cores to use")

args = parser.parse_args()

EXE_PATH = args.exe_path
print('Creating batch script for LiveOcean forecast run for:')
print(EXE_PATH)

NP_NUM = args.np_num

#N_NODES = int(NP_NUM /28)
N_NODES = int(NP_NUM /32)

## create lo_batch.sh - batch job script  ##########################

f  = open('lo_back_BLANK.sh','r')
f2 = open('lo_back_batch.sh','w')

in_varlist = ['EXE_PATH', 'N_NODES']

for line in f:
    for var in in_varlist:
        if '$'+var+'$' in line: 
            line2 = line.replace('$'+var+'$', str(eval(var)))
            line = line2
        else:
            line2 = line
    f2.write(line2)

f.close()
f2.close()
