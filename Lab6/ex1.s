/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  for (i = 0, p = nums; i != 4; i++, p++)
    printf("%d\n", *p);
  return 0;
}
*/

#Seguimento de dados (inicializados)
.data
nums:  .int  10, -21, -30, 45
Sf:  .string "%d\n"    # string de formato para printf

# Seguimento de texto
.text
Lab14
# definição de um símbolo global
.globl  main

#símbolo main representando o ponto de entrada do código
main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)
/********************************************************/

  movl  $0, %ebx  /* ebx = 0; ebx como variável i */
  movq  $nums, %r12  /* r12 = &nums r12 é a variável p*/

L1:
  cmpl  $4, %ebx  /* if (ebx == 4) ? */
  je  L2          /* goto L2 */

  movl  (%r12), %eax    /* eax = *r12 */

/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq    $Sf, %rdi    /* primeiro parametro (ponteiro)*/
  movl    %eax, %esi   /* segundo parametro  (inteiro) */
  call  printf       /* chama a funcao da biblioteca */
/*************************************************************/

  addl  $1, %ebx  /* ebx += 1; i++ */
  addq  $4, %r12  /* r12 += 4; p++ */
  jmp  L1         /* goto L1; */

L2:  
/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret      
/***************************************************************/