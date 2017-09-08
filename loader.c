#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>

// Simple-1: gcc -c -O simple-1.c
/* char shellcode[] = "\xb8\x2a\x00\x00\x00\xc3"; */
/* int offset = 0; */
// Simple-2: gcc -c -O simple-2.c
char shellcode[] = "\xb8\x01\x00\x00\x00\x85\xff\x74\x0f\x53\x89\xfb\x8d\x7f\xff\xe8\xec\xff\xff\xff\x0f\xaf\xc3\x5b\xf3\xc3\x41\x54\x55\x53\x85\xff\x7e\x20\x41\x89\xfc\xbb\x00\x00\x00\x00\xbd\x00\x00\x00\x00\x89\xdf\xe8\xca\xff\xff\xff\x01\xc5\x83\xc3\x01\x41\x39\xdc\x75\xef\xeb\x05\xbd\x00\x00\x00\x00\x89\xe8\x5b\x5d\x41\x5c\xc3";
int offset = 26;

typedef int (*shellcode_t)(int);
  
int main() {
  int size = sizeof(shellcode)-1;
  char *src_ptr = shellcode;
  void *code_ptr = valloc(size);
  memmove(code_ptr, src_ptr, size);
  int err = mprotect(code_ptr, size, 0x7);
  if (0 != err) {
    printf("mprotect: %d\n", err);
    return 1;
  }
  shellcode_t f = code_ptr+offset;
  int result = f(5);
  free(code_ptr);
  printf("%d\n", result);
  return 0;
}
