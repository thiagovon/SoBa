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

mov %edi, %eax
inc %eax

leave
ret

