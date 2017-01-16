#include <stdlib.h>
#include <stdio.h>

typedef  int (*mult_funct_t)(int, int);


void print_array(int *array, size_t size)
{
	int i;

	for (i = 0; i < size - 1; ++i)
		printf("%d, ", array[i]);
	printf("%d\n", array[size - 1]);
}

size_t nb_figure(int n)
{
	size_t nb = 0;

	while (n) {
		nb++;
		n = n / 10;
	}

	return nb;
}

int *nb_to_int_array(int n)
{
	size_t size = nb_figure(n);
	int *array = malloc(size * sizeof(int));

	if (array ==  NULL)
		return NULL;

	size_t i = 0;

	while (n) {
		array[i] = n % 10;
		n = n / 10;
		++i;
	}

	return array;
}

int int_array_to_nb(int *array, size_t size)
{
	int i;
	int n = 0;

	for (i = size-1; i >= 0; --i)
		n = n * 10 + array[i];

	return n;
}

int naive_mult(int a, int b)
{
	int i, j;
	int nb_a = nb_figure(a);
	int nb_b = nb_figure(b);
	int nb_p = nb_a + nb_b + 1;

	int *line = calloc(nb_p, sizeof(int));

	if (line == NULL)
		return 0;

	int *array_a = nb_to_int_array(a);
	int *array_b = nb_to_int_array(b);

	if (array_a == NULL || array_b == NULL)
		return 0;

	for (i = 0; i < nb_b; i++) {
		for (j = 0; j < nb_a; ++j) {
			// product
			int p = array_b[i] * array_a[j];

			// addition
			size_t idx = i+j;
			int tmp = line[idx] + p;

			// report carry if needed
			while (tmp > 10) {
				line[idx] = tmp % 10;
				idx++;
				tmp = line[idx] + tmp/10;
			}
			line[idx] = tmp;
		}
	}

	int ret = int_array_to_nb(line, nb_p);

	free(array_a);
	free(array_b);
	free(line);

	return ret;
}

int test_mult(mult_funct_t mult)
{
	int ret;
	int expect;
	int i, j;
	int max = 10000;
	int prev_i = 1;

	for (i = 0; i < max; ++i) {
		if (i / prev_i >= 10) {
			printf("i=%d\n", i);
			prev_i = i;
		}
		for (j = 0; j < max; ++j) {
			ret = mult(i, j);
			expect = i * j;
			if (ret != expect) {
				printf("%d != %d * %d = %d\n", ret, i, j, expect);
				return 1;
			}
		}
	}

	return 0;
}

int main(void)
{
	mult_funct_t mult = naive_mult;

	if (test_mult(mult))
		return 1;

	return 0;
}
