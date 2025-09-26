/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  int sum = 0;
  for (i = 0, p = nums; i != 4; i++, p++)
    sum += *p;
  printf("%d\n", sum);
  return 0;
}
*/

.data
nums:  .int  10, -21, -30, 45
Sf:  .string "%d\n"    # string de formato para printf

.text
.globl  main
main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)
/********************************************************/

  movl  $0, %ebx  /* ebx = i = 0; */
  movq  $nums, %r12  /* r12 = p = &nums */
  movl  $0, %r13d   /* r13d = sum = 0; Acumulador da Soma*/

L1:
  cmpl  $4, %ebx  /* if (ebx == 4) ? */
  je  L2          /* goto L2 */

/* sum += *p; */
  movl  (%r12), %eax      /* eax = *r12 = *p (Carrega o valor do array) */
  addl  %eax, %r13d       /*sum += eax (Adiciona a soma em r13d) */
  addl  $1, %ebx          /* i++; */
  addq  $4, %r12          /* p++; (avança 4 bytes) */
  jmp L1                  /* Volta para o inicio do loop */

/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq    $Sf, %rdi    /* primeiro parametro (ponteiro)*/
  movl    %eax, %esi   /* segundo parametro  (inteiro) */
  call  printf       /* chama a funcao da biblioteca */
/*************************************************************/

  addl  $1, %ebx  /* ebx += 1; */
  addq  $4, %r12  /* r12 += 4; */
  jmp  L1         /* goto L1; */

L2:  
  /* IMPRESSAO FINAL: printf("%d\n", sum); */
  movq  $Sf, %rdi         /* 1o arg: string de formato Sf */
  movl  %r13d, %esi       /* 2o arg: o valor final da soma (sum) */
  movl  $0, %eax          /* para chamadas variadic */
  call  printf
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret      
/***************************************************************/