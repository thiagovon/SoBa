#include <stdio.h>

unsigned char switch_byte(unsigned char x) {
  unsigned char low_nibble = (x & 0x0F) << 4;
  unsigned char high_nibble = (x & 0xF0) >> 4;
  return low_nibble | high_nibble;
}

unsigned char rotate_left(unsigned char x, int n) {
  unsigned char shifted_left = x << n;
  unsigned char lost_bits = x >> (8 - n);
  return shifted_left | lost_bits;
}

int main() {
  printf("--- Testes da funcao switch_byte ---\n");
  
  unsigned char test_value1 = 0xAB;
  printf("O valor original 0x%X se torna 0x%X (esperado: 0xBA)\n", test_value1, switch_byte(test_value1));

  unsigned char test_value2 = 0xCD;
  printf("O valor original 0x%X se torna 0x%X (esperado: 0xDC)\n", test_value2, switch_byte(test_value2));
  
  printf("\n--- Testes da funcao rotate_left ---\n");
  
  unsigned char test_value_r = 0x61;
  printf("Valor original: 0x%X\n", test_value_r);
  
  unsigned char result_r1 = rotate_left(test_value_r, 1);
  printf("Rotate left de 1 bit: 0x%X (esperado: 0xC2)\n", result_r1);
  
  unsigned char result_r2 = rotate_left(test_value_r, 2);
  printf("Rotate left de 2 bits: 0x%X (esperado: 0x85)\n", result_r2);
  
  unsigned char result_r3 = rotate_left(test_value_r, 7);
  printf("Rotate left de 7 bits: 0x%X (esperado: 0xB0)\n", result_r3);
  
  return 0;
}