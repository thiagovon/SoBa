int main (void) {
  int i,j;
  short a[2][3];
  int b[2];

  for (i=0;i<2;i++) {
    b[i] = i;
    for (j=0;j<3;j++)
      a[i][j] = 3*i+j;
  }

  printf ("b: \n");
  dump (b, sizeof(b));
  printf ("a: \n");
  dump (a, sizeof(a));

  return 0;
}