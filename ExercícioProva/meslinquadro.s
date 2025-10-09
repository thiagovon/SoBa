/*
struct S{
    int a;
    int *z;
    int b;
}

int buba (struct s *p, int n){
    int soma = 0;
    for (i = 0, i < m; i++){
        printf("%d", p[i].b)
        if (p[i].z != NULL)
            soma += p[i].a;
    }
    return soma;
}
*/

.text
.globl buba

buba:
    pushq   %rbp
    movq    %rsp, %rbp
    subq    $32, %rsp
    movq    -8(%rbp), %r12
    movq    -16(%rbp), %r13
    movq    -24(%rbp), %r15
    movq    %rdi, %r15

# int soma = 0;
    movl    $0, %r12d 

# i = 0
    movl    $0, %r13d 

for:
# i < m
    jae     %esi, %r13d

# printf("%d", p[i].b
    movl    16(%r15), %edi
    call    print

# if (p[i].p != NULL)
    cmpq    $0, 8(%r15) 
    je      cond
# soma += p[i].a
    addl    (%r15), %r12d

cond:
    addl    $1, %r13d
    addq    
    jmp     for



/*
Dicionário
int b       %edi
struct s    %rdi
int n       %esi
int soma    %r12d
int i       %r13d
tamanho struct = 20 
moveu p para r15
*/   