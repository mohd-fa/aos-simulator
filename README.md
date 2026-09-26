 # AOS

## Overview

This repository contains the AOS assignment codebase, which includes the implementation of a simple operating system simulator. The project is structured to facilitate the creation and management of processes, scheduling, and resource allocation.

## How to Run

### Compile the Code
```bash
gcc -c time.c
gcc -c process.c
gcc -c os.c
gcc -c print.c
gcc -c schedulingAlgo.c
gcc main.c time.o process.o os.o print.o schedulingAlgo.o -o osSimulator

```

### Execute the Simulator
```bash
./osSimulator <No_of_Processes> 
```

you can change the schecduling algorithm by changing the `SCHEDFUNC` definition in `ScisSos.h` file with any function names in the `schedulingAlgo.c` file.
