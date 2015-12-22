#include <stdio.h>
#include <stdlib.h>

int main()
{
    int m = 0, n =9;
    int k = m+n;
    printf("k is %d\n", k);
    m = k + n;
    printf("m is %d\n", m);
    n = m-n;
    printf("n is %d\n", n);
    return 0;
}

