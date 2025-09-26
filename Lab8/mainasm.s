/*
#include <stdio.h>
#define LIM 1

int filtro(int x, int lim) {
  if (x < lim)
    return 0;
  else
    return x;
}

int nums[5] = {3,-5,7,8,-2};

int main() {
  int i;
  int *p = nums;
  for (i = 0; i < 5; i++) {
    printf("%d\n", filtro(*p,LIM));
    p++;
  }
  return 0;
}
*/


  .data
  nums: .int 3, -5, 7, 8, -2
  s1:   .string "%d\n"

  .text /* estamos na área de texto */
  .globl main /* função global main */
  main:
  /* prologo */
     pushq %rbp
     movq  %rsp, %rbp
     subq  $16, %rsp
     movq  %rbx, -8(%rbp)
     movq  %r12, -16(%rbp)

  /* coloque seu codigo aqui */
  movl   $0, %ebx /* i = 0 */
  movq  $nums, %r12   /* p = nums */

WHILE:
  /* while (i<5) */
  cmpl   $5, %ebx 
  jge FORA_WHILE

  /* temp = filtro(*p, LIM) */
  /* printf("%d\n", temp) */
  movl   (%r12), %edi /* primeiro argumento */
  movl   $1, %esi     /* segundo argumento */
  call filtro

  movq  $s1, %rdi /* primeiro argumento de printf */
  movl  %eax, %esi /* segundo argumento de printf */
  movl  $0, %eax  /* caso especial para printf */
  call printf

  addq   $4, %r12 /* p++ */
  incl   %ebx
  jmp    WHILE

FORA_WHILE:
  movl   $0, %eax /* return 0 */

  /* finalizacao */
     movq -8(%rbp), %rbx
     movq -16(%rbp), %r12
     leave
     ret

