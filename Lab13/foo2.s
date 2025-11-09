/*
int foo2 (int x) {
  return add(x);
}

Dicionário:
x   edi
*/

.text
.globl foo

foo2:

  pushq %rbp
  movq %rsp, %rbp

# edi já está como primeiro parâmetro e o eax como valor retornado
  call add

  leave
  ret