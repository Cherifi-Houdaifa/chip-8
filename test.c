#include <stdio.h>
#include <stdlib.h>

int main () {
	long x = 0xf0ff;
	printf("0x%lx\n", x);
	int n = 56;
	x = (x << n) | (x >> (64 - n));
	printf("0x%lx\n", x);


}
