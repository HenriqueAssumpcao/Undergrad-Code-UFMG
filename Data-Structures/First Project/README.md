# First Project in Data Structures
In order to run this code, follow these two steps:
1. Assert that your directory is organized according to the following structure,i.e. create obj/ and bin/ directories:
```
First Project/
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
>>>./bin/run.out <TARGET_FILE>
```
2.2. Or the following in Windows:
```
>>> make 
>>>.\bin\run.out <TARGET_FILE>
```
Where <TARGET_FILE> stands for the .txt containing the commands for the program. You can also run ```make clean``` in order to delete the compiled files created by make.


The /tests directory contain a .cpp file that performs basic structural tests using doctest. These files are not automatically compiled by Make and so in order to run them, compile them manually in the respective folder.
