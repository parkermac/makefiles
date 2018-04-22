"""
This creates a batch script for LiveOcean -- using a template version of 
the batch file replacing things like $whatever$ with actual values and paths.
-dsd- November 2015

"""

# get command line arguments
import argparse

parser = argparse.ArgumentParser()

# positional arguments
parser.add_argument("exe_path", type=str, help="output path to dot.in file")

#parser.add_argument("gridname", type=str, help="cascadia1, etc.")
#parser.add_argument("tag", type=str, help="base, etc.")
#parser.add_argument("date_string", type=str, help="e.g. 2014.02.14")
# and this is an optional input parameter
#parser.add_argument("-x", "--ex_name", type=str, help="e.g. lo1")

args = parser.parse_args()

# setup
import os; import sys

EXE_PATH = args.exe_path
print(' \nCreates batch script for LiveOcean forecast run for ' + EXE_PATH + ' \n')

## create lo_batch.sh - batch job script  ##########################

f  = open('lo_back_BLANK.sh','r')
f2 = open('lo_back_batch.sh','w')

in_varlist = ['EXE_PATH']

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
