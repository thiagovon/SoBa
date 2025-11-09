/*
int foo (int x) {
  return x+1;
}

Dicionário:
x   edi
*/

.text
.globl foo

foo:
  pushq %rbp
  movq %rsp, %rbp

#Incrementa o primeiro argumento da função
  incl %eax

#Valor de retorno
  movl %edi, %eax

  leave
  ret

