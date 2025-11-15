/*
double foo (double a, int b) {
  return a * (-b);
}

Dicionário:
double a    xmm0
int b       edi
*/

.text
.globl foo

foo:
    pushq   %rbp
    movq    %rsp, %rbp

    neg     %edi
    cvtsi2sd    %edi, %xmm1
    mulsd   %xmm1, xmm0

    leave 
    ret
    