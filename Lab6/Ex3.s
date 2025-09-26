/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  for (i = 0, p = nums; i != 4; i++, p++)
    if ((*p % 2) == 0)
      printf("%d\n", *p);
  return 0;
}
*/

.data
nums:  .int 10, -21, -30, 45
Sf:    .string "%d\n"       # string de formato para printf

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
  movl  $0, %r13d         /* r13d = 0 (Inicializa o acumulador de *p) */
  movl  $0, %ebx          /* ebx = i = 0; */
  movq  $nums, %r12       /* r12 = p = &nums */

L1:
  cmpl  $4, %ebx          /* if (ebx == 4) ? */
  jge   L2                /* goto L2 */

  movl  (%r12), %r13d     /* r13d = *p; Salvando o valor original */
  movl  %r13d, %eax       /* temp = *p; copia para o teste */
  andl  $1, %eax          /* testa se é par (and & 1) */
  cmpl  $0, %eax          /* if resultado == 0 (par)? */
  jne   L_incrementa      /* Se for ímpar, pula a impressão e vai pro incremento */

/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq  $Sf, %rdi         /* primeiro parametro (ponteiro)*/
  movl  %r13d, %esi       /* segundo parametro (usa o valor SALVO em r13d) */
  movl  $0, %eax          /* Zera %eax para chamada de printf */
  call  printf            /* chama a funcao da biblioteca */
/*************************************************************/

L_incrementa:             /* Rótulo para pular o printf se for ímpar */
  addl  $1, %ebx          /* ebx += 1; */
  addq  $4, %r12          /* r12 += 4; */
  jmp   L1                /* goto L1; */

L2: 
/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax          /* rax = 0  (valor de retorno) */
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret                     
/***************************************************************/