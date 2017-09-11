from ctypes import (CDLL, c_long, c_char_p, c_void_p, memmove, cast, CFUNCTYPE, c_uint64, Structure, c_int, c_char, create_string_buffer)

#from shellcode_bytes import shellcode, offset
shellcode = b'\x55\x48\x89\xe5\xb8\x2a\x00\x00\x00\x5d\xc3\x55\x48\x89\xe5\xb8\x00\x00\x00\x00\xba\x00\x00\x40\x00\xff\xd2\x48\x98\x5d\xc3';
offset = 11
#offset = 26
entry_address = 0x400000

MAP_FLAGS = 0x32

def execute():
    libc = CDLL('libc.so.6')

    src_ptr = c_char_p(shellcode)
    size = len(shellcode)
    
    code_ptr = libc.mmap(entry_address, size, 0x7, MAP_FLAGS, -1, 0)
    if (code_ptr == -1):
        libc.err(1, "mmap failed")
        return ""
    code_ptr = c_void_p(code_ptr)
    memmove(code_ptr, src_ptr, size)

    main_ptr = c_void_p(c_uint64(code_ptr.value).value + c_uint64(offset).value)
    SHELLCODE_T = CFUNCTYPE(c_long)
    
    fptr = cast(main_ptr, SHELLCODE_T)
    result = fptr()
    #fptr(fen, move, result_buf)
    # libc.free(code_ptr)
    return result

print(execute())
