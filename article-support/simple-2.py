from ctypes import *
shellcode = b'\xb8\x01\x00\x00\x00\x85\xff\x74\x0f\x53\x89\xfb\x8d\x7f\xff\xe8\xec\xff\xff\xff\x0f\xaf\xc3\x5b\xf3\xc3\x41\x54\x55\x53\x85\xff\x7e\x20\x41\x89\xfc\xbb\x00\x00\x00\x00\xbd\x00\x00\x00\x00\x89\xdf\xe8\xca\xff\xff\xff\x01\xc5\x83\xc3\x01\x41\x39\xdc\x75\xef\xeb\x05\xbd\x00\x00\x00\x00\x89\xe8\x5b\x5d\x41\x5c\xc3'
offset = 26 # Decimal conversion of 1a, the beginning of custom_main

libc = CDLL('libc.so.6')

def get_executable_buffer(shellcode):
    src_ptr = c_char_p(shellcode)
    size = len(shellcode)

    code_ptr = libc.valloc(size)
    code_ptr = c_void_p(code_ptr)

    memmove(code_ptr, src_ptr, size)
    err = libc.mprotect(code_ptr, size, 0x7)
    if 0 != err:
        raise Exception("mprotect: " + str(code))
    return code_ptr, size

def execute():
    code_ptr, size = get_executable_buffer(shellcode)

    main_ptr = c_void_p(c_uint64(code_ptr.value).value + c_uint64(offset).value)
    SHELLCODE_T = CFUNCTYPE(c_int, c_int)
    
    fptr = cast(main_ptr, SHELLCODE_T)
    result = fptr(5)
    libc.free(code_ptr)
    return result

print execute()
