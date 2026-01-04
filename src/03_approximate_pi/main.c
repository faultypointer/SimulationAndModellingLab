#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ITERATIONS 10000000

int main()
{
	int n = 0;
	int N = 1;
	while (N <= ITERATIONS) {
		float x = rand() / (float)RAND_MAX;
		float y = rand() / (float)RAND_MAX;
		float f_x = sqrtf(1 - x * x);
		if (y <= f_x) {
			n++;
		}
		N++;
	}
	printf("Approximated Value of pi: %f\n", 4.f * n / N);
	return 0;
}
