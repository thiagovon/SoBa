/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  for (i = 0, p = nums; i != 4; i++, p++)
    printf("%d\n", *p);
  return 0;
}
*/

.data
nums:  .byte 10, -21, -30, 45  /* MODIFICADO: Array de 1 byte (char) */
Sf:    .string "%d\n"

.text
.globl main
main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq %rbp
  movq  %rsp, %rbp
  subq  $16, %rsp
  movq  %rbx, -8(%rbp)
  movq  %r12, -16(%rbp)
/********************************************************/

  movl  $0, %ebx        /* ebx = i = 0; */
  movq  $nums, %r12     /* r12 = p = &nums */

L1:
  cmpl  $4, %ebx        /* if (ebx == 4) ? */
  je  L2                /* goto L2 */

  /* MODIFICADO: Carrega 1 byte com extensão de sinal para 32 bits */
  movsbl (%r12), %eax   /* eax = *p (Lê o byte e estende o sinal) */

/*************************************************************/
/* este trecho imprime o valor de %eax (nao estraga %eax)     */
  movq  $Sf, %rdi       /* primeiro parametro (ponteiro)*/
  movl  %eax, %esi      /* segundo parametro (o valor de 32 bits) */
  movl  $0, %eax        /* Zera %eax para chamada de printf */
  call  printf          /* chama a funcao da biblioteca */
/*************************************************************/

  addl  $1, %ebx        /* ebx += 1; (i++) */
  addq  $1, %r12        /* MODIFICADO: r12 += 1; (p++ agora avanca 1 byte) */
  jmp L1                /* goto L1; */

L2: 
/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax        /* rax = 0 (valor de retorno) */
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret                   
/***************************************************************/