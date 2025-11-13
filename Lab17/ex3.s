/*
double foo (double a, int b) {
  return a * (-b);
}
struct X {
  int vi;
  double vd;
  struct X *next;
};

double pack (struct X *px) {
  double acc = 0;
  while (px != NULL) {
    acc += foo(px->vd, px->vi);
    px = px->next;
  }
  return acc;
}

Dicionário
struct X *px        rdi
px->vd (double a)   xmm0
px->vi              edi

*/

.globl
.text pack

pack:
    pushq   %rbp
    movq    %rsp, %rbp
    subq    ????, %rsp

# double acc = 0;
    movq    $0, $r10
    cvtsi2sd    $r10, $xmm0

WHILE:
    cmpq    $0, $rdi
    je      FIM

    #salvando callleer-saved
    movq    %rdi, -8(%rbp)
    movsd   %xmm0, -16(%rbp)

# acc += foo(px->vd, px->vi);

    movsd   8($rdi), $xmm0  # xmm0 (primeiro argumento) recebe px->vd 
    movl    (%rdi), $edi    # edi (primeiro argumento) recebe px->vi 
    call foo

    addsd   -16(%rbp), %xmm0

     movq   -8(%rbp), %rdi
   
# px = px->next 
    movq    16(%rdi), $rdi
    jmp WHILE

FIM:    
    leave
    ret