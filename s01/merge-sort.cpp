/**
 * week 1 - part 1
 * merge sort algorithm
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

void print_array(int *array, size_t size)
{
  size_t i = 0;
  for (i = 0; i < size - 1; ++i)
    printf("%d, ", array[i]);
  printf("%d\n", array[size - 1]);
}

void merge(int *array, size_t size)
{
  int *tmp = (int *)malloc(size * sizeof(int));
  if (tmp == NULL)
    return;

  size_t middle = size/2;
  size_t i = 0;
  size_t j = middle;
  size_t k = 0;
  for (k = 0; k < size; ++k) {
    if (i == middle) {
      tmp[k] = array[j];
      ++j;
    } else if (j == size) {
      tmp[k] = array[i];
      ++i;
    } else if (array[i] < array[j]) {
      tmp[k] = array[i];
      ++i;
    } else {
      tmp[k] = array[j];
      ++j;
    }
  }
  memcpy(array, tmp, size * sizeof(int));
}

void merge_sort(int *array, size_t size)
{
  if (size < 2) {
    return;
  } else if (size == 2) {
    if (array[0] > array[1]) {
      int tmp = array[0];

      array[0] = array[1];
      array[1] = tmp;
    }
  } else {
    size_t middle = size/2;

    merge_sort(array, middle);
    merge_sort(array + middle, size - middle);
    merge(array, size);
  }
}

int test_merge_sort(const char* name, int *input_array, int *expected_array,
            size_t size)
{
  merge_sort(input_array, size);

  if (memcmp(input_array, expected_array, size*sizeof(input_array[0]))) {
    printf("%s:%s: result != expected:\n", __func__, name);
    print_array(input_array, size);
    printf("while expecting:\n");
    print_array(expected_array, size);
    return 1;
  }
  return 0;
}

int test_merge_sort_set_size_1(void)
{
  int input_array[] = {8};
  int expected_array[] = {8};
  size_t size = ARRAY_SIZE(input_array);

  return test_merge_sort("size_1", input_array, expected_array, size);
}

int test_merge_sort_set_size_2_sorted(void)
{
  int input_array[] = {8, 11};
  int expected_array[] = {8, 11};
  size_t size = ARRAY_SIZE(input_array);

  return test_merge_sort("size_2_sorted", input_array, expected_array, size);
}

int test_merge_sort_set_size_2_unsorted(void)
{
  int input_array[] = {11, 6};
  int expected_array[] = {6, 11};
  size_t size = ARRAY_SIZE(input_array);

  return test_merge_sort("size_2_unsorted", input_array, expected_array,
    size);
}

int test_merge_sort_odd_set(void)
{
  int input_array[] = {8, 5, 7, 86, 4, 23, 10, 1, 0};
  int expected_array[] = {0, 1, 4, 5, 7, 8, 10, 23, 86};
  size_t size = ARRAY_SIZE(input_array);

  return test_merge_sort("odd_set", input_array, expected_array, size);
}

int test_merge_sort_even_set(void)
{
  int input_array[] = {8, 5, 7, 86, 78, 4, 23, 10, 1, 0};
  int expected_array[] = {0, 1, 4, 5, 7, 8, 10, 23, 78, 86};
  size_t size = ARRAY_SIZE(input_array);

  return test_merge_sort("even", input_array, expected_array, size);
}

int main(void)
{
  test_merge_sort_set_size_1();
  test_merge_sort_set_size_2_sorted();
  test_merge_sort_set_size_2_unsorted();
  test_merge_sort_even_set();
  test_merge_sort_odd_set();

  return 0;
}
