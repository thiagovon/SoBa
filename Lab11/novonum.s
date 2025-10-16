/*
int novonum(void) {
  int minhalocal;
  printf("numero: ");
  scanf("%d",&minhalocal);
  return minhalocal;
}
*/

.data 
printa: .string "numero: "
reada: .string "%d"

.text
.globl novonum

novonum:
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %r12, -4(%rbp)

  movq    $printa, %rdi
  movl    $0, %eax          #excessão printf, mover $0 para o %eax
  call    printf

# scanf("%d",&minhalocal);
  movq    $reada, %rdi
  leaq    -4(%r12d), %rsi
  call    scanf

  movl    -4(%rbp), %eax
  leave
  ret
