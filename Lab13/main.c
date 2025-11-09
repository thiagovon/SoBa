#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_START(P) ((intptr_t)(P)&~(pagesize-1))
#define PAGE_END(P) (((intptr_t)(P)+pagesize-1)&~(pagesize-1))

typedef int (*funcp) (int x);

int foo(int x);
int bar (int x);
int execpage(void *ptr, size_t len);

unsigned char codigo[] = {
   0x55,                      // push   %rbp
   0x48, 0x89, 0xe5,          // mov    %rsp,%rbp
   0x89, 0xf8,                // mov    %edi,%eax
   0xff, 0xc0,                // inc    %eax
   0xc9,                      // leave
   0xc3                       // ret
};

int bar (int x) {
  return x-1;
}

int main(void) {
    funcp f;
    int a;
    int res;

    if (execpage(codigo, sizeof codigo)) {
        fprintf(stderr, "Error changing memory permissions\n");
        exit(1);
    }

    a = 123;
    res = foo(a);
    printf("foo(%d) = %d\n", a, res);

    a = 123;
    res = bar(a);
    printf("bar(%d) = %d\n", a, res);

    f = foo;
    a = 321;
    res = f(a);
    printf("f(%d) = %d\n", a, res);

    f = bar;
    a = 321;
    res = f(a);
    printf("f(%d) = %d\n", a, res);

    f = (funcp) codigo;
    a = 456;
    res = f(a);
    printf("codigo(%d) = %d\n", a, res);

    return 0;
}

int foo (int x) {
  return x+1;
}

/*
 * The execpage() function shall change the specified memory pages
 * permissions into executable.
 *
 * void *ptr  = pointer to start of memory buff
 * size_t len = memory buff size in bytes
 *
 * The function returns 0 if successful and -1 if any error is encountered.
 * errono may be used to diagnose the error.
 */
int execpage(void *ptr, size_t len) {
	int ret;

	const long pagesize = sysconf(_SC_PAGE_SIZE);
	if (pagesize == -1)
		return -1;

	ret = mprotect((void *)PAGE_START(ptr),
		 PAGE_END((intptr_t)ptr + len) - PAGE_START(ptr),
		 PROT_READ | PROT_WRITE | PROT_EXEC);
	if (ret == -1)
		return -1;

	return 0;
}