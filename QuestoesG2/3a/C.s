/*
float fooC (int a, float b) {
  return a + cos(b);
}

Dicionário
int a       edi
float b    xmm0
*/

.text
.globl fooC

fooC:
    pushq   %rbp
    movq    %rsp, %rbp

    cvtsi2sd    %edi, %xmm2
    cvtss2sd    %xmm0, %xmm0

    call    cos

    addssd  %xmm2, %xmm0

    cvtsd2ss    %xmm0, %xmm0

    leave 
    ret

ou


.text
.globl fooC

fooC:
    pushq   %rbp
    movq    %rsp, %rbp

    cvtsi2sd    %edi, %xmm1
    cvtss2sd    %xmm0, %xmm0

    movssd      %xmm1, -8(%rbp)
    call    cos

    addssd  -8(%rbp), %xmm0

    cvtsd2ss    %xmm0, %xmm0


    leave 
    ret


CONFIRMAR COM O MESLIN SOBRE A UTILIZAÇÃO DE XMM2 AO INVÉS DE XMM1 E NÃO PRECISAR SALVAR NO RA
