#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("FIFO_%d\n",getpid());
    return 0;
}
