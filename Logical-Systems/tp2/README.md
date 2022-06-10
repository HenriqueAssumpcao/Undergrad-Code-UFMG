# Second Project in Verilog
Second project in Verilog, containing implementations of three different registers.

This folder contains one file :
1. [Docs.pdf](Docs.pdf) -> The documentation for the project

The subfolder src/ contains four files:
1. [test_bench_1.v](src/test_bench_1.v) -> Test bench for exercise 1
2. [test_bench_2.v](src/test_bench_2.v) -> Test bench for exercise 2
3. [test_bench_3.v](src/test_bench_3.v) -> Test bench for exercise 3
4. [shift_left_reg.v](src/shift_left_reg.v) -> Module for shift-left 4bit register (exercise 1)
5. [feedback_register.v](src/feedback_register.v) -> Module for 4bit feedback register (exercise 2)
6. [johnson_counter.v](src/johnson_counter.v) -> Module for johnson counter (exercise 3)

In order to run the code, go to the src/ directory and do the following:
```
>>> iverilog -o test_bench_<NUM>.vvp test_bench_<NUM>.v
>>> vvp test_bench_<NUM>.vvp
>>> gtkwave
```
Where <NUM> indicates the respective exercise(1,2 or 3).
# Dependencies
[Icarus Verilog](http://iverilog.icarus.com/)
