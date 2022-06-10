# Second Project in Data Structures
In order to run this code, follow these two steps:
1. Assert that your directory is organized according to the following structure,i.e. create obj/ and bin/ directories:
```
Second Project/
└── src/
    └── main.cpp
└── include/
    └── buffer.h
└── obj/
└── bin/
└── Makefile
```
2.1. Run the following in command prompt on Linux:
```
>>> make 
>>>./bin/run.out <TARGET_FILE> <SORT_MODE> <NUM_LINES>
```
2.2. Or the following in Windows:
```
>>> make 
>>>.\bin\run.out <TARGET_FILE> <SORT_MODE> <NUM_LINES>
```
Where <TARGET_FILE> stands for the .txt containing the commands for the program, <SORT_MODE> refers to one of the four available configurations for sorting the file(see documentation for more information) and <NUM_LINES> represents the number of lines to be read from the target file.
You can also run ```make clean``` to delete the files created by make.
