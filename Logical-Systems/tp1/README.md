# First Project in Verilog
This folder contains an implementation of a XOR gate in verilog. The gate is tested in the context of ciphering strings of 32 bits given an OTP(One Time Pad), i.e. perform a bit-wise XOR operation between each bit of the OTP and the string. This returns a ciphered string of 32 bits that can be deciphered performing the XOR operation with the same OTP.

This folder contains one file :
1. [Docs.pdf](Docs.pdf) -> The documentation for the project

The subfolder src/ contains two files:
1. [xor_logic.v](src/xor_logic.v) -> Module for the XOR gate
2. [codificador_tb.v](src/codificador_tb.v) -> The test bench that performs the ciphering and deciphering.

In order to run the code, go to the src/ directory and do the following:
```
>>> iverilog -o codificador_tb.vvp codificador_tb.v
>>> vvp codificador_tb.vvp
```
Optionally, after running the .vvp file, you can use a wave viewer such as [GTKWave](http://gtkwave.sourceforge.net/) to access the wave patterns of the program:
```
>>> gtkwave
```
Inside gtkwave, open the codificador_tb.vcd file.
# Dependencies
[Icarus Verilog](http://iverilog.icarus.com/)
