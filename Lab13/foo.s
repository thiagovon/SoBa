/*
int foo (int x) {
  return x+1;
}
*/

.text
.globl foo

foo:

pushq %rbp
movq %rsp, %rbp

#Valor de retorno
movl %edi, %eax

#Incrementa o primeiro argumento da função
incl %eax

leave
ret

