# Multi-threaded Matrix Operations ##

This is a simple multi threaded version of matrix operations, that utilizes the pthread library.

## Specifications ##
This program supports:
+ Matrix addition
+ Matrix subtraction
+ Matrix multiplication

## Multi-threading ##
+ For addition and subtraction, each pair of corresponding elements are added in a single thread.
+ For multiplication, a thread is spawned for each dot product needed to be computed.

## Compiling the Program ##
The program can be complied by running the `make` command.
