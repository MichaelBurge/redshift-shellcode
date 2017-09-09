from ctypes import (CDLL, c_long, c_char_p, c_void_p, memmove, cast, CFUNCTYPE, c_uint64, Structure, c_int, c_char, create_string_buffer)

from shellcode_bytes import shellcode, offset
#shellcode = b'\xb8\x2a\x00\x00\x00\xc3'
#offset = 26

class GAMESTATE(Structure):
    _fields_ = [("rooks", c_uint64),
                ("knights", c_uint64),
                ("bishops", c_uint64),
                ("queens", c_uint64),
                ("kings", c_uint64),
                ("pawns", c_uint64),
                ("current_player", c_uint64),
                ("en_passant_sq", c_int),
                ("castle_flags", c_uint64)]

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
    fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    #move = "a1b1"
    result_buf = create_string_buffer(300)
    SHELLCODE_T = CFUNCTYPE(None, c_char_p, c_char_p, c_int)
    
    fptr = cast(main_ptr, SHELLCODE_T)
    fptr(fen, result_buf, 2)
    #fptr(fen, move, result_buf)
    libc.free(code_ptr)
    return result_buf.raw

print execute()
