#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline int max(int a, int b)
{
	return (a > b) ? a : b;
}

typedef char *(*mult_funct_t)(const char *, const char *);

typedef struct {
	size_t size;
	char *tab;
} array_number_t;

void array_number_print(const array_number_t *n)
{
	size_t i;
	size_t size = n->size;

	for (i = 0; i < size-1; ++i)
		printf("%d, ", n->tab[i]);
	printf("%d\n", n->tab[size-1]);
}

array_number_t *array_number_create(array_number_t *n, size_t size)
{
	n->size = size;
	n->tab = calloc(n->size, sizeof(*(n->tab)));

	if (n->tab == NULL) {
		printf("%s: calloc failed for size:%u\n", __func__,
			n->size * sizeof(*(n->tab)));
		return NULL;
	}

	return n;
}

array_number_t *array_number_create_from_string(array_number_t *n,
												const char *s)
{
	size_t size = strlen(s);

	n->size = size;
	n->tab = calloc(n->size, sizeof(*(n->tab)));
	if (n->tab == NULL)
		return NULL;

	size_t i = 0;

	for (i = 0; i < size; ++i)
		n->tab[size - 1 - i] = s[i] - '0';

	return n;
}

char *array_number_to_string(const array_number_t *n)
{
	// get first non null value
	size_t size = n->size;

	while ((size - 1 != 0) && (n->tab[size - 1] == 0))
		size--;

	// adding 1 char to ensure c-string null termination
	char *s = malloc((size + 1) * sizeof(char));

	if (s == NULL) {
		printf("%s: cannot allocate string\n", __func__);
		return NULL;
	}

	size_t i;

	for (i = 0; i < size; ++i)
		s[i] = n->tab[size-1-i] + '0';
	s[size] = '\0';

	return s;
}

char *naive_mult(const char *string_a, const char *string_b)
{
	array_number_t a;
	array_number_t b;
	array_number_t product;

	if (array_number_create_from_string(&a, string_a) == NULL) {
		printf("unable to create a\n");
		return NULL;
	}
	if (array_number_create_from_string(&b, string_b) == NULL) {
		printf("unable to create b\n");
		return NULL;
	}
	if (array_number_create(&product, a.size + b.size + 2) == NULL) {
		printf("unable to create product\n");
		return NULL;
	}
	
	int i, j;
	
	for (i = 0; i < b.size; i++) {
		for (j = 0; j < a.size; ++j) {
			// product
			char p = a.tab[j] * b.tab[i];

			// addition, with carry report if needed
			size_t idx = i+j;
			char tmp = product.tab[idx] + p;

			while (tmp >= 10) {
				product.tab[idx] = (tmp % 10);
				idx++;
				tmp = product.tab[idx] + tmp/10;
			}
			product.tab[idx] = tmp;
		}
	}

	// reconvert to ascii
	char *ret = array_number_to_string(&product);

	free(a.tab);
	free(b.tab);
	free(product.tab);

	return ret;
}

/**
 * a * b = (c*10^i + d) * (e*10^i + f)
 *       = ce*10^i2 + (cf + de)*10^i + df
 *
 *    99 = 81*100 + 162*10 + 81 =    81
 *    99 =                         1620
 *   891                           8100
 *  891                            9801  
 *  9801 
 **/
/*int divide_conquer_mult(int a, int b)
{
	if (a < 10 && b < 10)
	{
		return a * b;
	}
	
	int size_a = nb_figure(a);
	int size_b = nb_figure(b);
	int i = max(size_a, size_b) / 2;

	int *array_a = nb_to_int_array(a);
	int d = int_array_to_nb(&array_a[0], i);
	int c = int_array_to_nb(&array_a[i], size_a-i);
	free(array_a);

	int *array_b = nb_to_int_array(b);
	int f = int_array_to_nb(&array_b[0], i);
	int e = int_array_to_nb(&array_b[i], size_b-i);
	free(array_b);

	printf("a=%d, b=%d\n", a, b);
	printf("c=%d, d=%d, e=%d, f=%d\n", c, d, e, f);
	
	int ce = divide_conquer_mult(c, e);
	int cf = divide_conquer_mult(c, f);
	int de = divide_conquer_mult(d, e);
	int df = divide_conquer_mult(d, f);

	int size = size_a + size_b + 2;
	int *array_ret = calloc(size_a + size_b + 2);
	int *array_tmp = nb_to_int_array(df);
	int j;
	for (j=0; j<size; ++j)
		array_ret[j] = array_tmp[j];
	free(array_tmp);
	array_ret = nb_to_int_array(cf);
	int carry = 0;
	for (j = i; j < size; ++j)
	{
		int tmp = array_ret[j] + 
	}
	free(array_ret);

	return 0;
}*/

int test_mult(mult_funct_t mult)
{
	size_t i, j;
	size_t max = 10000;
	size_t prev_i = 1;
	char string_i[max];
	char string_j[max];
	size_t max_expect = 2*max+2;
	char string_expect[max_expect];

	for (i = 0; i < max; ++i) {
		snprintf(string_i, max, "%u", i);
		if (i / prev_i >= 10) {
			printf("i=%d\n", i);
			prev_i = i;
		}
		for (j = 0; j < max; ++j) {
			snprintf(string_j, max, "%u", j);
			char* ret = mult(string_i, string_j);
			int expect = i * j;
			snprintf(string_expect, max_expect, "%u", expect);
			if (strcmp(ret, string_expect)) {
				printf("%s != %d * %d = %s\n", ret, i, j, string_expect);
				free(ret);
				return 1;
			}
			free(ret);
		}
	}

	return 0;
}

int main(void)
{
	char *ret = naive_mult("1", "10");
	printf("%s : %d * %d = %d\n", ret, 1, 10, 1*10);
	free(ret);

	char a[] = "3141592653589793238462643383279502884197169399375105820974944592";
	char b[] = "2718281828459045235360287471352662497757247093699959574966967627";
	char *ret2 = naive_mult(a, b);
	printf("%s * %s = %s\n", a, b, ret2);
	free(ret2);
	
	/*if (test_mult(naive_mult))
		return 1;*/

	/*if (test_mult(divide_conquer_mult))
		return 1;*/
	return 0;
}
