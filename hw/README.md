#ex2
This is a exercise on how to use pointer pointer as well as pointer to implement a linked list that can have data field 
of int, double and char.\
It is also possible to sort the linked list by different orders, increase, decrease or random on data field.

## How to compile
gcc -o ll linkedlist.c

## Input & How to run
run with ./ll rand_[int/double/char].txt [inc/dec/rand]

[int/double/char].txt is the the file that one wants to sort\
It has the format of key=data on each line

## Output
./ll will output the sorted file named [inc/dec/rand]_[int/double/char].txt