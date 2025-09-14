#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main (void) {
  signed char sc = -1;
  unsigned int ui = sc;

  printf("O valor de sc e: %d\n", sc);
  printf("O valor de ui e: %u\n", ui);
  printf("\nDump da variavel sc (8 bits):\n");
  dump(&sc, sizeof(sc));
  printf("\nDump da variavel ui (32 bits):\n");
  dump(&ui, sizeof(ui));

  return 0;
}