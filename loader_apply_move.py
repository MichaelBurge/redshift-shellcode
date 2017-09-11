from ctypes import (CDLL, c_long, c_char_p, c_void_p, memmove, cast, CFUNCTYPE, c_uint64, c_int, c_char, create_string_buffer)

from shellcode_bytes import shellcode, offset
#shellcode = b'\xb8\x2a\x00\x00\x00\xc3'
#offset = 26
fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
move = "e1f1"

def execute():
    libc = CDLL('libc.so.6')

    src_ptr = c_char_p(shellcode)
    size = len(shellcode)

    code_ptr = libc.valloc(size)
    code_ptr = c_void_p(code_ptr)

    memmove(code_ptr, src_ptr, size)
    err = libc.mprotect(code_ptr, size, 0x7)
    if 0 != err:
        raise Exception("mprotect: " + str(code))

    main_ptr = c_void_p(c_uint64(code_ptr.value).value + c_uint64(offset).value)
    
    result_buf = create_string_buffer(300)
    SHELLCODE_T = CFUNCTYPE(None, c_char_p, c_char_p, c_char_p)
    
    fptr = cast(main_ptr, SHELLCODE_T)
    fptr(fen, move, result_buf)
    libc.free(code_ptr)
    return result_buf.raw

print execute()
