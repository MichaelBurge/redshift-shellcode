long y() { return 42; }
void* custom_main() { return &y; }

/* int y() { return 42; } */
/* typedef int (*shellcode_t)(); */
/* long main() { return ((shellcode_t)0x400000)(); } */
