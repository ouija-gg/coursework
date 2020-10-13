#include <time.h>
#include <stdio.h>

int main() {
    time_t t = time(NULL);
    printf ("tiem is %s", asctime(gmtime(&t)));
    return 0;
}
