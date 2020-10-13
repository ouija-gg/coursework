#### compile #####
gcc -std=C99 -o server queue.c server.c
gcc -std=C99 -o client client.c
### or use Makefile #####
make clean && make

#### running ####
./server
(in a seperate window) ./client <some number>

OR
./server
make test
