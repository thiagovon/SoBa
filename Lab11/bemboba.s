/*
int bemboba (int num) {
  int local[4];
  int *a;
  int i;

  for (i=0,a=local;i<4;i++) {
    *a = num;
    a++;
  }
  return addl (local, 4);
}
*/

.text
.globl bemboba

bemboba:
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp

# i=0
	movl $0, %r10d
# a=local
	leaq -16(%rbp), %r11

for:
# i<4
  cmpl  $4, %r10d
  jge   fora_for

# *a = num;
  movl %edi, (%r11)
# a++
  addq  $4, %r11
# i++
  incl %r10d
  jmp  for

fora_for:

# rdi (primeiro parâmetro) = local
  leaq -16(%rbp), %rdi

# esi (segundo parametro) = 4
	movl $4, %esi 
	call addl 

fim:
  leave
  ret


