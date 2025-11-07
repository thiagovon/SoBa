/*
double foo2 (float a, float b) {
  return sin(a) + cos(b);
}

Dicionário
float a   $xmm0
float b   $xmm1
*/

.text
.globl foo2

foo2:
  pushq $rbp
  movq  $rsp, $rbp
  subq  ????, $rsp

# como as funções sin e cos recebem um double, precisamos converter a e b
  cvtss2sd  $xmm0, $xmm0
  cvtss2sd  $xmm1, $xmm1

# Função sin (salvar o valor de xmm1 para não perdê-lo)
  movsd $xmm1, -8($rbp)
  call sin
# Guarda o resultado de sin
  movsd $xmm0, -16($rbp)

# Função cos (passar o valor de b para o primeiro argumento)
  movsd $-8($rbp), %xmm0
  call cos

# sin(a) + cos(b)
  addsd -16(%rbp), %xmm0

  leave
  ret