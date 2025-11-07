/*
double foo3 (double *a, int n) {
  int i;
  double r = 0.0;
  for (i=0; i<n; i++) {
    r += sin(*a);
    a++;
  }
  return r;
}

Dicionário
double *a   %rdi
double r    %xmm1
int n       %esi
int i       %edx
*/

.text
.globl foo3

foo3:
    pushq   %rbp
    movq    %rsp, %rbp
    subq    $32, %rsp

# int n;

#COMEÇO:
# double r = 0.0;
    movl        $0, %edi
    cvtsi2sd    %edi, %xmm1

# for (i=0; i<n; i++) {
# i=0
    movl    $0, %edi

# i<n
    cmpl    %esi, %edx
    jge     FORA_FOR

# r += sin(*a);
# Salvar para não perder valores
    movq    %rdi, -8(%rbp)
    movsd   %xmm1, -16(%rbp)
    movl    %esi, -20(%rbp)
    movl    %edx, -24(%rbp)

# chamar função e sin(*a)
    movsd   (%rdi), %xmm0
    call    sin

# recuperar valores
    movq    -8(%rbp), %rdi
    movsd   -16(%rbp), %xmm1
    movl    -20(%rbp), %esi
    movl    -24(%rbp), %edx

    addsd    %xmm0, %xmm0   # r += sin(*a)
    
    incl    %edx            # i++
    addq    $8, %rdi        # a++ (tem que adicionar 8 por ser um ponteiro)

FORA_FOR:
    leave
    ret