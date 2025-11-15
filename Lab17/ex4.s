/*
struct X1 {
  int vi;
  float vf;
};

double pack1 (struct X1 *px, int n) {
  double acc = 0;
  int i;
  for (i=0;i<n;i++)
    acc += foo(px[i].vf, px[i].vi);
  return acc;
}

Dicionário
struct X1 *px   rdi (ponteiro)
int n           esi
*/

.text
.globl pack1

pack1:
    pushq   %rbp
    movq    %rsp, %rbp
    subq    ????, %rsp

    movq    $0, $r10
    cvtsi2sd    $r10, $xmm0     #double acc = 0
    movl    $0, $r11d

WHILE:




