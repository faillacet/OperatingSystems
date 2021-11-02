# OSAssignment4
* Trenton Faillace

# Compile Instructions
* g++ prodcon.cpp -o prodcon -lpthread

# Run Instructions
* ./prodcon nitems ntimes
* nitems - amounts of items in bounded buffer (may not be larger than 1024)
* ntimes - amounts of times this program reads/writes to the bounded buffer
*  OR
* ./prodcon filename nitems ntimes
* filename - name of file in same directory that you wish to use to generate shared memory