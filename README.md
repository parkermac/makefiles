# README for makefiles

### Each folder contains the files used to compile ROMS for one of our machines, e.g. mox and klone. The folder is also where oceanM, the executable, ends up after compiling.

---

#### lo8k
This is the first one for compiling on klone.
- It is similar to lo8 (mox) in that it is for LO_ROMS but with the bio flags turned off.
- It also is the first use of NetCDF4 in ROMS.
- This is also among the first instances where we moved all the sbatch file code to LO/dot_in/shared.

---

#### lo8kb
Like lo8k but with the bio flags turned on.

Note: to make a new instance you just copy the .h and makefile from the an old folder to a new one (lo8k to lo8kb in this case).

Then rename the .h, in this case to lo8kb.h.

Then make these few edits in the .h:
```
#define BIOLOGY
#define NPZD2O_BANAS
```
And make these slight name changes in two lines of makefile:
```
ROMS_APPLICATION ?= LO8KB
MY_HEADER_DIR ?= /gscratch/macc/parker/LiveOcean_roms/makefiles/lo8kb
```

---

#### n0k
For a nested run on klone.  This is based on lo8k but with no tides.  To remove the tidal forcing we undefine these flags in the .h:

```
#undef SSH_TIDES
#undef UV_TIDES
#undef ADD_FSOBC
#undef ADD_M2OBC
```
and I am trying these (2021.09.28):
```
#define DEFLATE
#define HDF5
```
---

#### u0mb
Just like the lo8b that is running the current forecast, but with NetCDF4 and compressed output.  Also uses the cleaner treatment of the batch files that I developed above.

Note that the makefile for this only differs from one I would use on klone by a single line: around 152 which has:
```
FORTM ?= ifort_mox
```

---

#### u0kb
Just like u0mb but for klone, so the .h files are identical except for the name, and the makefiles are similar except for the names and the FORTM line.

The is meant to run the current forecast, but on klone, and with compressed NetCDF4 output.

---
