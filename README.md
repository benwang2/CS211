# CS211 Computer Architecture
C programming assignments from Rutgers CS 211, Fall 2021.

## About
This repository contains code for 4 of 5 assignments given. The missing assignment is a variant of the binary bomb lab and as a result, there is no written C program.

___
## Brief Overview

### PA1: C Programming [(pa1)](https://github.com/benwang2/CS211/tree/main/pa1)
The assignment was to write 6 relatively short programs.
1. yell.c
    * Write a program yell that prints its argument all-caps. yell will read its first argument and print
it to standard output, replacing all lower-case characters with their corresponding upper-case letter.
After printing the modified string, it will print two exclamation points and a newline.
2. rle.c
    * Write a program rle that uses a simple method to compress strings. rle takes a single argument and looks for repeated characters. Each repeated sequence of a letter or punctuation mark is reduced to a single character plus an integer indicating the number of times it occurs. Thus, “aaa” becomes “a3” and “ab” becomes “a1b1”. If the compressed string is longer than the original string, rle must print the original string instead.
3. balance.c
    * Write a program balance that checks whether a string contains correctly nested and balanced parentheses, brackets, and braces. Your program will take a single argument and analyze whether each open delimiter has a corresponding closing delimiter of the correct type.
4. list.c
    * Write a program list that maintains and manipulates a sorted linked list according to instructions received from standard input. The linked list is maintained in order, meaning that the items in the list are stored in increasing numeric order after every operation. **list** supports two operations, **insert** *n* and **delete** *n*.
5. mexp.c
    * Write a program mexp that multiplies a square matrix by itself a specified number of times. mexp takes a single argument, which is the path to a file containing a square (k * k) matrix M and a non-negative exponent n. It computes Mn and prints the result.
6. bst.c
    * Write a program bst that manipulates binary search trees. It will receive commands from standard input, and print resposes to those commands to standard output. You will implement bst with three commands: **insert** *n*, **search** *n*, and **print**.

<sub>To view the full description, see [pa1.pdf](https://github.com/benwang2/CS211/tree/main/pa1/pa1.pdf).</sub>

___
### PA2: One-shot Learning [(pa2)](https://github.com/benwang2/CS211/tree/main/pa2)
Given matrices *X* and *Y*, your program will compute *(X<sup>T</sup>  X)<sup>-1</sup>X<sup>T</sup>Y* in order to learn *W*. This will require (1) multiplying, (2) transposing, and (3) inverting matrices.

<sub>To view the full description, see [pa2.pdf](https://github.com/benwang2/CS211/tree/main/pa2/pa2.pdf).</sub>

___
### PA4: Digital Logic [(pa4)](https://github.com/benwang2/CS211/tree/main/pa4)
You will write a program truthtable that reads a file containing a description of a circuit, and prints that circuit’s truth table. The files specify (1) the number and names of each *input* to the
circuit, (2) the number and names of each *output* from the circuit, and (3) the logic gates and components that make up the circuit.

<sub>To view the full description, see [pa4.pdf](pa4/pa4.pdf).</sub>

___
### PA5: Cache Simulation [(pa5)](https://github.com/benwang2/CS211/tree/main/pa5)
Write a cache simulator that will simulate two memory operations: reading and writing individual bytes. Your program will read a trace file describing addresses to read or write from, and will keep track of which blocks are stored in which cache lines in order to determine when these memory operations result in actual reads and writes to memory.

<sub>To view the full description, see [pa5.pdf](https://github.com/benwang2/CS211/tree/main/pa5/pa5.pdf).</sub>
