#include <stdio.h>
#include <stdlib.h>

#define N 5
void foo (int a[], int n);
int main() {
  // Teste: {10, 0, 5, 0, 2}
  // Resultado esperado: {10, 10, 5, 5, 2}
  int nums[N] = {10, 0, 5, 0, 2};
  int i;

  printf("Array Original: {10, 0, 5, 0, 2}\n");

  foo(nums, N);

  printf("Array Modificado: {");
  for (i = 0; i < N; i++) {
    printf("%d%s", nums[i], (i < N - 1) ? ", " : "");
  }
  printf("}\n");

  return 0;
}