# Third Project in Verilog
Third project in Verilog, containing implementations of three different versions of a LFSR.

This folder contains one pdf file :
1. [Docs.pdf](Docs.pdf) -> The documentation for the project

The subfolder src/ contains four files:
1. [test_bench_3bit.v](src/test_bench_3bit.v) -> Test bench for 3-BIT LFSR
2. [test_bench_4bit.v](src/test_bench_4bit.v) -> Test bench for 4-BIT LFSR
3. [test_bench_8bit.v](src/test_bench_8bit.v) -> Test bench for 8-BIT LFSR
4. [lfsr.v](src/lfsr.v) -> File containing all three implementations of the LFSR's

The /images folder contains multiple .png files contain the bitmaps and plots of the patterns for each LFSR.

In order to run the code, go to the src/ directory and do the following:
```
>>> iverilog -o test_bench_<NUM>bit.vvp test_bench_<NUM>.v
>>> vvp test_bench_<NUM>bit.vvp
>>> gtkwave
```
Where <NUM> indicates the number of bits of the LFSR, i.e. 3, 4 or 8.
# Dependencies
[Icarus Verilog](http://iverilog.icarus.com/)
