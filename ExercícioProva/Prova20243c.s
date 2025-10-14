*/

int buba(char *v, int n, char c)
{
int i, s = 0;
for (i=0; i<n; i++)
{
if (v[i] <= c)
s += corta(v[i], n);
}
return s;
}

char *v %rdi
int n   %esi
char c  %dl
int s   %ebx
int i   %r12d
*/

.text
.globl buba

buba:

pushq   %rbp
movq    %rsp, %rbp
subq    $16, %rsp
movq    %rbx, -8(%rbp)
movq    %r12, -16(%rbp)

#int i, s = 0;
movl    $0, %ebx
movl    $0, %r12d

for:
#for (i=0; i<n; i++)

#i<n
cmpl    %esi, %r12d
jge     FIM


#if (v[i] <= c)
movq    %rdi, %r10
addq    %r12d, %r10
movb    (%r10), %ec
cmpb    %dl, %ec

#s += corta(v[i], n);
movb    %dl, %dil
call    corta
addl    %eax, %ebx

INCREMENTO:
incl    %r12d
jmp     for

FIM:
leave
ret





mov