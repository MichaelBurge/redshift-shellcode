from ctypes import (CDLL, c_long, c_char_p, c_void_p, memmove, cast, CFUNCTYPE, c_uint64, Structure, c_int)

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
    # New game values
    g = GAMESTATE(
        rooks   = 9295429630892703873,
        knights = 4755801206503243842,
        bishops = 2594073385365405732,
        queens  = 576460752303423496,
        kings   = 1152921504606846992,
        pawns   = 71776119061282560,
        current_player  = 65535,
        en_passant_sq = 255,
        castle_flags = 15)
    SHELLCODE_T = CFUNCTYPE(c_uint64, GAMESTATE)
    fptr = cast(main_ptr, SHELLCODE_T)
    result = fptr(g)
    libc.free(code_ptr)
    return result

print execute()
