/*
void foo (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++) {
    s += a[i];
    if (a[i] == 0) {
      a[i] = s;
      s = 0;
    }
  }
}
*/

.text
.globl foo

foo:
# void foo (int a[], int n) {
    pushq   %rbp        # 1 - salva a base do RA 
    movq    %rsp, %rbp  # 2 - cria a base do RA da função chamada
    subq    $16, %rsp   #abre espaço para o RA da função chamada
    movq    %r12, -8(%rbp)  #salva o valor do r15 no RA

# int i;
# int s = 0;
    movl    $0, %ecx


# i=0 
    movl    $0, %edx

for:   
#  for (i=0; i<n; i++)
# i<n
    cmpl    %esi, %edx
    jge     FIM

# s += a[i];
    movl    (%rdi), %r12d
    addl     %r12d, %ecx

# if (a[i] == 0) 
    cmpl    $0,   %r12d 
    jne     volta_for

# a[i] = s;
    movl    %ecx, %r12d

# s = 0;
    movl    $0,   %ecx
    jmp     volta_for

volta_for:
# i++
    incl    %edx
    addq    $4,    %rdi
    jmp     for
    
FIM:
    movq    -8(%rbp), %r12   #inverter linha 23
    leave 
    ret

/*
Dicionário
int a[]     rdi 8 bytes
int n       esi 4 bytes
int i       edx 4 bytes
int s       ecx 4 bytes
a[i]        r12d
*/