*/
float foo (double a, float b) {
  return (a+b)*(a-b);
}

Dicionário:
double a  xmm0
float b   xmm1
/*

.text
.globl foo

foo:
  pushq %rbp
  movq  %rsp, %rbp
  
  cvtss2sd  $xmm1, $xmm1

# Preciso guardar o valor em xmm2 para poder preservar o valor para efetuar as duas operações
  movsd     $xmm0, $xmm2

# (a + b)
  addsd     $xmm1, $xmm0

# (a - b)
  subsd     $xmm1, $xmm2

# (a+b) * (a-b)
  mulsd     $xmm2, $xmm0


# Como a função é float, o resultado final precisa ser convertido
  cvtsd2ss  $xmm2, $xmm2

  leave
  ret

