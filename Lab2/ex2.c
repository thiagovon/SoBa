#include <ctype.h>
#include <stdio.h>

int string2num (char *s, int base) {
  int a = 0;
  for (; *s; s++) {
    int digit_value;
    if (*s >= '0' && *s <= '9') {
      digit_value = *s - '0';
    } else {
      digit_value = *s - 'a' + 10;
    }
    a = a * base + digit_value;
  }
  return a;
}

int main(void) {
  printf("%d\n", string2num("1a", 16));
  printf("%d\n", string2num("a09b", 16));
  printf("%d\n", string2num("z09b", 36));
  return 0;
}