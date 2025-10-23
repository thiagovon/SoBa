/*
int foo (int x) {
  return add(x);
}
*/

.text
.globl foo

foo:
    pushq %rbp
	movq %rsp, %rbp
    call add
    call sub
    call mult
    call div,
    leave
    ret