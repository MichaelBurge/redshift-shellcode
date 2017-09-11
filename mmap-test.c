// To manually assign an entry address: gcc mmap-test.c -Wl,-Ttext-segment=0x8000000

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

char shellcode[] = "\x55\x48\x89\xe5\xb8\x2a\x00\x00\x00\x5d\xc3\x55\x48\x89\xe5\xb8\x00\x00\x00\x00\xba\x00\x00\x00\x40\xff\xd2\x48\x98\x5d\xc3";
int offset = 11;
void *entry_address = (void*)(0x40000000);

typedef int (*shellcode_t)();

int main()
{
  char *src_ptr = shellcode;
  int size = sizeof(shellcode)-1;
  void *code_ptr = mmap(entry_address, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
  if (code_ptr == MAP_FAILED) {
    err(1, "mmap failed");
    return 1;
  }
  memmove(code_ptr, src_ptr, size);
  shellcode_t f = (shellcode_t)(code_ptr + offset);
  int result = f();
  printf("%d\n", result);
  return 0;
}
