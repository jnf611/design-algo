/**
 * week 1 - part 1
 * simple (naive) multiplication
 **/
#include <stdio.h>
#include <stdlib.h>

int mult(int a, int b)
{
	if (a < 10 && b < 10) {
		return a*b;
	} else if (a < 10) {
		return mult(a, b) + mult();
	} else if (b < 10) {
		return mult(a/10) +;
	} else {
		
	}
		
	return a*b;
}

int main()
{
	int a = 23;
	int b = 12;
	int p = mult(a, b);
	printf("%d*%d = %d", a, b, p);
	
	return 0;
}
