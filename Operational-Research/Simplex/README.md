# Simplex
This folder contains a simple python implementation of the Simplex algorithm. This implementation attempts to solve the LP via matrix inversion operations, instead of explicitly pivoting indexes.

The simplex algorithm solves a Linear Programming problem expressed as follows:

max c_1 * x_1 + c_2 * x_2 + ... + c_n * x_n

s.t. 

a_11 * x_1 + a_12 + x_2 + ... + a_1n * x_n <= b_1

.
.
.

a_m1 * x_1 + a_m2 + x_2 + ... + a_mn * x_n <= b_m


In order to run the program, do the following:
```
>>> python simplex.py
m n
c_1 c_2 c_3 ... c_n
a_11 a_12 a_12 ... a_1n b_1
...
a_m1 a_m2 a_m3 ... a_mn b_m
```

The possible outputs are:
* ("ótima",optimal_value,optimal_solution,optimal_certificate) -> The LP is optimal
* ("ilimitada",possible_solution,unlimited_certificate) -> The LP is unlimited
* ("inviável",unviable_certificate) -> The LP is unviable
