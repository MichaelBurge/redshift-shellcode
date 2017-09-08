from ctypes import *

shellcode = b'\xb8\x2a\x00\x00\x00\xc3'

libc = CDLL('libc.so.6')

src_ptr = c_char_p(shellcode)
size = len(shellcode)

code_ptr = libc.valloc(size)
code_ptr = c_void_p(code_ptr)

memmove(code_ptr, src_ptr, size)
err = libc.mprotect(code_ptr, size, 0x7)
if 0 != err:
    raise Exception("mprotect: " + str(code))

fptr = cast(code_ptr, CFUNCTYPE(c_long, c_long))
libc.free(code_ptr)
