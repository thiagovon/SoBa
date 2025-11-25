/*
struct X1 {
 int vi;
 float vf;
};
double pack1 (struct X1 *px, int n) {
 double acc = 0;
 int i;
 for (i=0;i<n;i++)
 acc += foo(px[i].vf, px[i].vi);
 return acc;

Dicionário:
struct X1 *px	rdi
int n		esi
*/

.text
.globl pack1

pack1:
pushq		%rbp
movq		%rsp, %rbp
subq		$32, %rsp

movq		$0, %r10
ctvsi2sd	%r10, %xmm0
movl		$0, %r11d	#int i

FOR:
cmpl		%esi, %r11d
jge		FIM

movsd		%xmm0, -8(%rbp)
movq		%rdi, -16(%rbp)
movl		%esi, -20(%rbp)
movl		%r11d, -24(%rbp)

imull		$8, %r11d	      #r11d(i) = r11d * 8 (tamanho da struct)
addq		%rdx, %rdi          /* rdi <- px[i] */
movl 		(%rdi), %edi        /* edi (primeiro argumento) = px[i].vi */
cvtss2sd 	4(%rdi), %xmm0  /* xmm0 (primeiro argumento) = (double) px[i].vf */

call foo

addsd -8(%rbp), %xmm0    /* xmm0 (valor de retorno de foo) += acc */
movq -16(%rbp), %rdi     /* restaura rdi */
movl -20(%rbp), %esi     /* restaura esi */
movl -24(%rbp), %r11d    /* restaura r11d */

incl %r11d               /* i++ */
jmp FOR

FIM:
leave
ret
