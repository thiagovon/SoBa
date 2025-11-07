*/
float foo1 (float a, float b) {
  return sin(a) + b;
}

Dicionário
float a   %xmm0
float b   %xmm1
/*

.text
.globl foo1

foo1:
  pushq %rbp
  movq  %rsp, %rbp
  subq  $16, %rsp

# como a função sin recebe um double, precisamos converter a e b
  cvtss2sd  $xmm0, $xmm0
  cvtss2sd  $xmm1, $xmm1

# a função sin guarda o valor de pi no xmm1. Por conta disso, precisamos guardá-lo para não perder.
  movsd $xmm1, -8(%rbp)
  call sin

# sin(a) + b
  addsd -8(%rbp), $xmm0

# a função foo1 é float, então precisamos converter 
  cvtsd2ss  $xmm0, $xmm0

  leave
  ret
