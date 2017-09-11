create or replace function pf_apply_move (fen varchar, move varchar) returns varchar stable as $$
from ctypes import (CDLL, c_long, c_char_p, c_void_p, memmove, cast, Structure, CFUNCTYPE, c_uint64, c_int, create_string_buffer)

def execute():
    shellcode = b'\x83\xfe\x03\x0f\x84\x67\x01\x00\x00\x7e\x55\x83\xfe\x05\x0f\x84\x1c\x01\x00\x00\x0f\x8c\xbe\x00\x00\x00\x83\xfe\x06\x0f\x85\xad\x00\x00\x00\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x30\x48\x89\x57\x28\x48\x89\x07\x48\x8b\x44\x24\x10\x48\x89\x47\x08\x48\x8b\x44\x24\x18\x48\x89\x47\x10\x48\x8b\x44\x24\x20\x48\x89\x47\x18\x48\x8b\x44\x24\x28\x48\x89\x47\x20\xeb\x49\x66\x0f\x1f\x44\x00\x00\x83\xfe\x01\x0f\x84\xa7\x00\x00\x00\x83\xfe\x02\x75\x62\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x10\x48\x89\x57\x08\x48\x89\x07\x48\x8b\x44\x24\x18\x48\x89\x47\x10\x48\x8b\x44\x24\x20\x48\x89\x47\x18\x48\x8b\x44\x24\x28\x48\x89\x47\x20\x48\x8b\x44\x24\x30\x48\x89\x47\x28\x48\x8b\x44\x24\x38\x48\x89\x47\x30\x48\x8b\x44\x24\x40\x48\x89\x47\x38\x48\x8b\x44\x24\x48\x48\x89\x47\x40\x48\x8b\x44\x24\x50\x48\x89\x47\x48\x48\x89\xf8\xc3\x0f\x1f\x44\x00\x00\x0f\x0b\x66\x0f\x1f\x44\x00\x00\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x20\x48\x89\x57\x18\x48\x89\x07\x48\x8b\x44\x24\x10\x48\x89\x47\x08\x48\x8b\x44\x24\x18\x48\x89\x47\x10\x48\x8b\x44\x24\x28\x48\x89\x47\x20\x48\x8b\x44\x24\x30\x48\x89\x47\x28\xeb\x94\x90\x48\x8b\x44\x24\x10\x48\x89\x54\x24\x08\x48\x89\x17\x48\x89\x47\x08\x48\x8b\x44\x24\x18\x48\x89\x47\x10\xe9\x59\xff\xff\xff\x90\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x28\x48\x89\x57\x20\x48\x89\x07\x48\x8b\x44\x24\x10\x48\x89\x47\x08\x48\x8b\x44\x24\x18\x48\x89\x47\x10\x48\x8b\x44\x24\x20\x48\x89\x47\x18\x48\x8b\x44\x24\x30\x48\x89\x47\x28\xe9\x39\xff\xff\xff\x66\x0f\x1f\x44\x00\x00\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x18\x48\x89\x57\x10\x48\x89\x07\x48\x8b\x44\x24\x10\x48\x89\x47\x08\xe9\xf9\xfe\xff\xff\x90\x49\x89\xca\x48\x63\x4c\x24\x08\x31\xc0\x48\x83\xf9\x3f\x77\x15\xb8\x01\x00\x00\x00\x48\xd3\xe0\x48\x89\xc1\xb8\x01\x00\x00\x00\x48\x85\xcf\x74\x0b\xf3\xc3\x66\x0f\x1f\x84\x00\x00\x00\x00\x00\x48\x85\xce\x74\x06\xb8\x02\x00\x00\x00\xc3\x48\x85\xca\x74\x06\xb8\x03\x00\x00\x00\xc3\x49\x85\xca\x74\x06\xb8\x04\x00\x00\x00\xc3\x49\x85\xc8\x74\x06\xb8\x05\x00\x00\x00\xc3\x4c\x21\xc9\x48\x83\xf9\x01\x19\xc0\xf7\xd0\x83\xe0\x06\xc3\x0f\x1f\x44\x00\x00\x41\x57\x41\x56\x45\x31\xff\x41\x55\x41\x54\x45\x31\xf6\x55\x53\x45\x31\xed\x45\x31\xe4\x31\xed\x31\xdb\x48\x81\xec\xe8\x01\x00\x00\x45\x31\xdb\x41\xb8\x01\x00\x00\x00\x64\x48\x8b\x04\x25\x28\x00\x00\x00\x48\x89\x84\x24\xd8\x01\x00\x00\x31\xc0\x48\x89\x14\x24\xba\x38\x00\x00\x00\x31\xc0\x0f\x1f\x84\x00\x00\x00\x00\x00\x48\x83\xc7\x01\x44\x0f\xb6\x4f\xff\x8d\x0c\x10\x48\x63\xc9\x41\x80\xf9\x42\x0f\x84\xd7\x01\x00\x00\x7f\x55\x41\x80\xf9\x33\x74\x42\x0f\x8e\x09\x01\x00\x00\x41\x80\xf9\x36\x74\x2d\x7f\x19\x41\x80\xf9\x34\x74\x2b\x41\x80\xf9\x35\x74\x22\x0f\x1f\x44\x00\x00\x0f\x0b\x66\x0f\x1f\x44\x00\x00\x41\x80\xf9\x37\x74\x09\x41\x80\xf9\x38\x75\xec\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\xeb\x92\x66\x90\x41\x80\xf9\x62\x0f\x84\x8d\x01\x00\x00\x7f\x3c\x41\x80\xf9\x50\x0f\x84\x32\x01\x00\x00\x0f\x8f\xec\x00\x00\x00\x41\x80\xf9\x4b\x74\x66\x41\x80\xf9\x4e\x75\xa8\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\xeb\x27\x0f\x1f\x80\x00\x00\x00\x00\x41\x80\xf9\x70\x0f\x84\x0d\x01\x00\x00\x0f\x8f\x98\x00\x00\x00\x41\x80\xf9\x6b\x74\x41\x41\x80\xf9\x6e\x0f\x85\x68\xff\xff\xff\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4d\x09\xce\xe9\x76\xff\xff\xff\x0f\x1f\x44\x00\x00\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4c\x09\xcd\xe9\x3f\xff\xff\xff\x66\x0f\x1f\x44\x00\x00\x41\x80\xf9\x2f\x0f\x84\x96\x08\x00\x00\x0f\x8e\x28\x01\x00\x00\x41\x80\xf9\x31\x0f\x84\x1f\xff\xff\xff\x41\x80\xf9\x32\x0f\x84\x12\xff\xff\xff\x0f\x0b\x66\x2e\x0f\x1f\x84\x00\x00\x00\x00\x00\x41\x80\xf9\x71\x74\x33\x41\x80\xf9\x72\x0f\x84\xd9\x00\x00\x00\x0f\x0b\x66\x0f\x1f\x44\x00\x00\x41\x80\xf9\x51\x0f\x85\xa6\x00\x00\x00\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4d\x09\xcc\xe9\xb5\xfe\xff\xff\x0f\x1f\x40\x00\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4c\x09\xcb\xe9\x7f\xfe\xff\xff\x66\x0f\x1f\x44\x00\x00\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4d\x09\xcd\xe9\x47\xfe\xff\xff\x66\x0f\x1f\x44\x00\x00\x41\x80\xf9\x52\x0f\x85\x0e\xfe\xff\xff\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4d\x09\xcf\xe9\x05\xfe\xff\xff\x0f\x1f\x40\x00\x41\x80\xf9\x20\x0f\x85\xce\xfd\xff\xff\x0f\xb6\x07\x3c\x62\x0f\x84\x6a\x07\x00\x00\x3c\x77\x0f\x85\xbb\xfd\xff\xff\xba\x01\x00\x00\x00\xc6\x44\x24\x08\x01\x48\x83\xc7\x02\x45\x31\xd2\x0f\xb6\x07\x3c\x20\x74\x29\x48\x83\xc7\x01\x3c\x51\x0f\x84\x67\x07\x00\x00\x0f\x8f\x49\x07\x00\x00\x3c\x2d\x74\xe3\x3c\x4b\x0f\x85\x85\xfd\xff\xff\x0f\xb6\x07\x49\x83\xca\x01\x3c\x20\x75\xd7\x84\xd2\x0f\x85\x81\x01\x00\x00\x0f\xb6\x7c\x24\x08\xc7\x84\x24\xc8\x00\x00\x00\xff\x00\x00\x00\x48\x8b\x84\x24\xc8\x00\x00\x00\x40\x88\xbc\x24\xd8\x00\x00\x00\x48\x89\x84\x24\x18\x01\x00\x00\x48\x8b\x84\x24\xd8\x00\x00\x00\xc7\x84\x24\x18\x01\x00\x00\xff\x00\x00\x00\x48\x89\x84\x24\x28\x01\x00\x00\x4c\x89\xf0\x40\x88\xbc\x24\x28\x01\x00\x00\x4c\x09\xf8\x49\x0f\xce\x49\x0f\xcf\x4c\x09\xe8\x49\x0f\xcd\x4c\x89\xbc\x24\xe0\x00\x00\x00\x4c\x09\xe0\x49\x0f\xcc\x4c\x89\xb4\x24\xe8\x00\x00\x00\x48\x09\xe8\x48\x0f\xcd\x4c\x89\xac\x24\xf0\x00\x00\x00\x48\x09\xd8\x48\x0f\xcb\x4c\x89\xa4\x24\xf8\x00\x00\x00\x49\x31\xc3\x4c\x89\xd0\x48\x89\xac\x24\x00\x01\x00\x00\x83\xe0\x01\x49\x0f\xcb\x48\x89\x9c\x24\x08\x01\x00\x00\x48\x83\xf8\x01\x4c\x89\x9c\x24\x10\x01\x00\x00\x4c\x89\xbc\x24\x30\x01\x00\x00\x19\xc9\xf7\xd8\x4c\x89\xb4\x24\x38\x01\x00\x00\x83\xe1\xfc\x83\xe0\x04\x4c\x89\xac\x24\x40\x01\x00\x00\x83\xc1\x0c\x41\xf6\xc2\x02\x4c\x89\xa4\x24\x48\x01\x00\x00\x0f\x45\xc1\x48\x89\xac\x24\x50\x01\x00\x00\x48\x89\x9c\x24\x58\x01\x00\x00\x89\xc1\x4c\x89\x9c\x24\x60\x01\x00\x00\x83\xc9\x01\x41\xf6\xc2\x04\x0f\x45\xc1\x89\xc1\x83\xc9\x02\x41\x83\xe2\x08\x0f\x45\xc1\x4c\x63\xd0\x48\x8b\x84\x24\x18\x01\x00\x00\x4c\x89\x94\x24\x20\x01\x00\x00\x4c\x89\x94\x24\x70\x01\x00\x00\x48\x89\x84\x24\x68\x01\x00\x00\x48\x8b\x84\x24\x28\x01\x00\x00\xc7\x84\x24\x68\x01\x00\x00\xff\x00\x00\x00\x48\x89\x84\x24\x78\x01\x00\x00\x48\x8b\x84\x24\x68\x01\x00\x00\x40\x88\xbc\x24\x78\x01\x00\x00\x48\x89\x84\x24\xc8\x00\x00\x00\x48\x8b\x84\x24\x78\x01\x00\x00\x48\x89\x84\x24\xd8\x00\x00\x00\x0f\xbe\x4e\x01\x0f\xbe\x06\x80\x7e\x04\x00\x8d\x84\xc8\x17\xfe\xff\xff\x0f\xbe\x4e\x03\x89\x44\x24\x10\x0f\xbe\x46\x02\x8d\x84\xc8\x17\xfe\xff\xff\x89\x44\x24\x18\x74\x27\x0f\xb6\x46\x05\x3c\x4e\x0f\x84\x98\x06\x00\x00\x0f\x8e\x29\x0a\x00\x00\x3c\x51\x0f\x84\x7c\x0a\x00\x00\x83\x4c\x24\x18\x40\x3c\x52\x0f\x85\x9f\xfb\xff\xff\x84\xd2\x75\x5d\x8b\x74\x24\x10\xb9\x07\x00\x00\x00\x89\xf2\x89\xf0\xc1\xfa\x1f\xc1\xea\x1d\x01\xd0\x83\xe0\x07\x29\xd0\x89\xf2\x83\xc2\x07\x85\xf6\x0f\x49\xd6\x89\xce\xc1\xfa\x03\x29\xd6\x8d\x04\xf0\x8b\x74\x24\x18\x89\x44\x24\x10\x89\xf2\x89\xf0\xc1\xfa\x1f\xc1\xea\x1d\x01\xd0\x83\xe0\x07\x29\xd0\x89\xf2\x83\xc2\x07\x85\xf6\x0f\x49\xd6\xc1\xfa\x03\x29\xd1\x8d\x04\xc8\x89\x44\x24\x18\x0f\xb6\x44\x24\x08\xc7\x84\x24\xc8\x00\x00\x00\xff\x00\x00\x00\x49\x89\xd9\x4c\x89\x94\x24\xd0\x00\x00\x00\x4c\x89\x54\x24\x30\x49\x89\xe8\x4c\x89\x9c\x24\xc0\x00\x00\x00\x4c\x89\xbc\x24\x90\x00\x00\x00\x4c\x89\xe1\x4c\x89\xb4\x24\x98\x00\x00\x00\x4c\x89\xac\x24\xa0\x00\x00\x00\x4c\x89\xea\x88\x84\x24\xd8\x00\x00\x00\x48\x8b\x84\x24\xc8\x00\x00\x00\x4c\x89\xf6\x4c\x89\xa4\x24\xa8\x00\x00\x00\x48\x89\xac\x24\xb0\x00\x00\x00\x4c\x89\xff\x48\x89\x9c\x24\xb8\x00\x00\x00\x48\x89\x84\x24\xb8\x01\x00\x00\x48\x8b\x84\x24\xd8\x00\x00\x00\x48\x89\x84\x24\xc8\x01\x00\x00\x8b\x44\x24\x10\x50\xe8\xa6\xf9\xff\xff\x8b\x74\x24\x20\x89\x44\x24\x28\x49\x89\xe8\x4c\x89\xe1\x49\x89\xd9\x4c\x89\xea\x4c\x89\xff\x89\xf0\xc1\xf8\x06\x89\x44\x24\x30\x89\xf0\x4c\x89\xf6\x83\xe0\x3f\x89\x44\x24\x20\x89\x04\x24\xe8\x72\xf9\xff\xff\x59\x85\xc0\x41\x89\xc0\x4c\x8b\x54\x24\x30\x0f\x85\x4a\x04\x00\x00\x4c\x89\xd6\xb8\xff\x00\x00\x00\x8b\x54\x24\x28\x85\xd2\x0f\x85\x05\x07\x00\x00\x8b\x7c\x24\x20\x83\xff\x03\x0f\x84\x72\x09\x00\x00\x0f\x8e\xdc\x08\x00\x00\x83\xff\x05\x0f\x84\x6b\x09\x00\x00\x0f\x8c\x6d\x09\x00\x00\x83\xff\x06\x48\x89\xda\x0f\x85\x1f\xfa\xff\xff\x4c\x63\x4c\x24\x10\x48\x63\x7c\x24\x18\x49\x83\xf9\x3f\x48\x89\x7c\x24\x28\x77\x10\x48\xc7\xc7\xfe\xff\xff\xff\x44\x89\xc9\x48\xd3\xc7\x48\x21\xfa\x0f\xb6\x4c\x24\x28\x89\x84\x24\xb8\x01\x00\x00\xbf\x01\x00\x00\x00\x0f\xb6\x44\x24\x08\x4c\x89\x54\x24\x38\x48\xd3\xe7\x48\x89\x7c\x24\x30\x88\x84\x24\xc8\x01\x00\x00\x48\x89\xf9\x48\x8d\x7c\x24\x40\xff\xb4\x24\xc8\x01\x00\x00\x56\xff\xb4\x24\xc8\x01\x00\x00\x41\x53\x48\x09\xca\x53\x55\x41\x54\x41\x55\x41\x56\x41\x57\x8b\x74\x24\x70\xe8\x11\xf7\xff\xff\x48\x8b\x8c\x24\xa0\x00\x00\x00\x48\x8b\x94\x24\xc8\x00\x00\x00\x48\x8b\xac\x24\xd0\x00\x00\x00\x4c\x8b\xa4\x24\xd8\x00\x00\x00\x48\x8b\xb4\x24\x90\x00\x00\x00\x4c\x8b\xac\x24\x98\x00\x00\x00\x4c\x8b\x9c\x24\xa8\x00\x00\x00\x48\x8b\x9c\x24\xb0\x00\x00\x00\x48\x8b\xbc\x24\xb8\x00\x00\x00\x48\x8b\x84\x24\xc0\x00\x00\x00\x45\x89\xe6\x48\x89\x8c\x24\xe0\x01\x00\x00\x48\x89\x94\x24\x08\x02\x00\x00\x48\x89\x4c\x24\x58\x48\x89\xb4\x24\xd0\x01\x00\x00\x89\xd1\x4c\x89\xac\x24\xd8\x01\x00\x00\x4c\x89\x9c\x24\xe8\x01\x00\x00\x48\x89\xea\x48\x89\x9c\x24\xf0\x01\x00\x00\x48\x89\xbc\x24\xf8\x01\x00\x00\x48\x89\x84\x24\x00\x02\x00\x00\x48\x89\xac\x24\x10\x02\x00\x00\x4c\x89\xa4\x24\x18\x02\x00\x00\x48\x83\xc4\x50\x4c\x8b\x54\x24\x38\x83\x7c\x24\x20\x06\x0f\x84\x14\x07\x00\x00\x83\x7c\x24\x20\x05\x40\x0f\x94\xc5\x83\x7c\x24\x18\x06\x0f\x94\xc1\x40\x20\xe9\x0f\x84\x8e\x08\x00\x00\x41\xf6\xc2\x01\x74\x12\x40\x80\xe6\x7f\x24\x7f\x48\x83\xe2\xfe\x48\x83\xce\x20\x48\x83\xc8\x20\x83\x7c\x24\x18\x02\x41\x0f\x94\xc7\x41\x20\xcf\x0f\x84\x54\x08\x00\x00\x41\x83\xe2\x02\x0f\x85\xf6\x05\x00\x00\xbd\xff\x00\x00\x00\x83\x7c\x24\x20\x01\x0f\x94\xc1\x83\x7c\x24\x10\x07\x41\x0f\x94\xc4\x41\x84\xcc\x74\x0e\x49\x89\xd4\x49\x83\xe4\xfe\xf6\xc2\x01\x49\x0f\x45\xd4\x44\x8b\x54\x24\x10\x45\x85\xd2\x41\x0f\x94\xc4\x41\x84\xcc\x74\x0e\x48\x89\xd1\x48\x83\xe1\xfd\xf6\xc2\x02\x48\x0f\x45\xd1\x41\x83\xf8\x01\x0f\x94\xc1\x83\x7c\x24\x18\x3f\x75\x0d\x49\x89\xd0\x49\x83\xe0\xfb\x84\xc9\x49\x0f\x45\xd0\x83\x7c\x24\x18\x38\x75\x0d\x49\x89\xd0\x49\x83\xe0\xf7\x84\xc9\x49\x0f\x45\xd0\x48\x89\xd1\x48\x83\xe1\xfc\x45\x84\xff\x48\x0f\x45\xd1\x49\x83\xf9\x3f\x77\x10\x49\xc7\xc0\xfe\xff\xff\xff\x44\x89\xc9\x49\xd3\xc0\x4c\x21\xc0\x4c\x8b\x7c\x24\x08\x48\x0b\x44\x24\x30\x41\x83\xf6\x01\x4d\x89\xe9\x45\x89\xf4\x41\xbe\xff\x00\x00\x00\x49\x0f\xc9\x4c\x89\xf9\x49\x0f\xcf\x4c\x09\xe9\x4c\x09\xd9\x48\x09\xd9\x48\x09\xf9\x48\x09\xf1\x48\x31\xc8\x48\x89\x4c\x24\x10\x4c\x89\xd9\x48\x0f\xc9\x49\x89\xc2\x81\xfd\xff\x00\x00\x00\x48\x89\x4c\x24\x18\x48\x89\xd9\x48\x89\xf0\x48\x0f\xc9\x4d\x89\xd0\x48\x0f\xc8\x48\x89\x4c\x24\x20\x48\x89\xf9\x49\x0f\xc8\x48\x0f\xc9\x48\x89\x4c\x24\x28\x74\x28\x8d\x4d\x07\x85\xed\x41\xbe\x07\x00\x00\x00\x0f\x49\xcd\xc1\xf9\x03\x41\x29\xce\x89\xe9\xc1\xf9\x1f\xc1\xe9\x1d\x01\xcd\x83\xe5\x07\x29\xcd\x46\x8d\x74\xf5\x00\x48\x89\xd1\x83\xe1\x01\x48\x83\xf9\x01\x19\xed\xf7\xd9\x83\xe5\xfc\x83\xe1\x04\x83\xc5\x0c\xf6\xc2\x02\x0f\x45\xcd\x89\xcd\x83\xcd\x01\xf6\xc2\x04\x0f\x45\xcd\x89\xcd\x83\xcd\x02\x83\xe2\x08\x0f\x45\xcd\x45\x84\xe4\x0f\x84\xc0\x04\x00\x00\x48\x63\xd9\x48\x89\x5c\x24\x10\x48\x8b\x2c\x24\x41\xba\x40\x00\x00\x00\xbb\x07\x00\x00\x00\x41\xbb\x01\x00\x00\x00\x44\x88\x64\x24\x08\x4d\x8d\x62\xf8\x31\xd2\x4c\x89\xe1\xeb\x79\x0f\x1f\x40\x00\x4d\x89\xdd\x49\xd3\xe5\x49\x85\xc5\x0f\x84\xd1\x01\x00\x00\x85\xd2\x0f\x84\xb2\x06\x00\x00\xbe\x01\x00\x00\x00\x83\xc2\x30\x4d\x85\xc5\x48\x8d\x7d\x01\x88\x55\x00\x0f\x95\xc2\x83\xfe\x04\x0f\x84\x31\x03\x00\x00\x0f\x8e\xef\x02\x00\x00\x83\xfe\x05\x0f\x84\x09\x03\x00\x00\x83\xfe\x06\x0f\x85\xe7\x02\x00\x00\x80\xfa\x01\x19\xd2\x83\xe2\x20\x83\xc2\x50\x0f\x1f\x84\x00\x00\x00\x00\x00\x48\x8d\x6f\x01\x88\x17\x31\xd2\x48\x83\xc1\x01\x4c\x39\xd1\x0f\x84\x86\x01\x00\x00\x48\x83\xf9\x3f\x76\x85\x83\xc2\x01\xeb\xe8\x83\xea\x08\x83\xfa\xf8\x0f\x85\x1a\xf6\xff\xff\x0f\xb6\x07\x3c\x62\x0f\x85\x96\xf8\xff\xff\x31\xd2\xc6\x44\x24\x08\x00\xe9\x9c\xf8\xff\xff\x0f\x1f\x44\x00\x00\x3c\x6b\x74\x24\x3c\x71\x0f\x85\x3c\xf6\xff\xff\x49\x83\xca\x08\xe9\x89\xf8\xff\xff\x0f\x1f\x00\x49\x83\xca\x02\xe9\x7d\xf8\xff\xff\x0f\x1f\x80\x00\x00\x00\x00\x49\x83\xca\x04\xe9\x6d\xf8\xff\xff\x83\xf8\x03\x0f\x84\x30\x05\x00\x00\x0f\x8e\x92\x04\x00\x00\x83\xf8\x05\x0f\x84\x19\x05\x00\x00\x0f\x8c\x0b\x05\x00\x00\x41\x83\xf8\x06\x48\x89\xd8\x0f\x85\xec\xf5\xff\xff\x0f\xb6\x74\x24\x08\x0f\xb6\x4c\x24\x18\x48\x8d\x7c\x24\x40\xc7\x84\x24\xb8\x01\x00\x00\xff\x00\x00\x00\x48\xc7\xc2\xfe\xff\xff\xff\x40\x88\xb4\x24\xc8\x01\x00\x00\xff\xb4\x24\xc8\x01\x00\x00\x48\xd3\xc2\x41\x52\x48\x21\xc2\x44\x89\xc6\x4c\x89\x54\x24\x40\xff\xb4\x24\xc8\x01\x00\x00\x41\x53\x53\x55\x41\x54\x41\x55\x41\x56\x41\x57\xe8\x04\xf3\xff\xff\x48\x8b\x94\x24\xd8\x00\x00\x00\x48\x8b\x84\x24\xc8\x00\x00\x00\x4c\x8b\xbc\x24\x90\x00\x00\x00\x4c\x8b\xb4\x24\x98\x00\x00\x00\x4c\x8b\xac\x24\xa0\x00\x00\x00\x4c\x8b\xa4\x24\xa8\x00\x00\x00\x48\x89\x84\x24\x08\x02\x00\x00\x48\x89\x94\x24\x18\x02\x00\x00\x48\x8b\xac\x24\xb0\x00\x00\x00\x48\x8b\x9c\x24\xb8\x00\x00\x00\x4c\x8b\x9c\x24\xc0\x00\x00\x00\x48\x8b\xb4\x24\xd0\x00\x00\x00\x88\x54\x24\x58\x48\x83\xc4\x50\x4c\x8b\x54\x24\x30\xe9\xc9\xfa\xff\xff\x81\x4c\x24\x18\x80\x00\x00\x00\xe9\x76\xf9\xff\xff\x0f\x1f\x44\x00\x00\x4d\x85\xcd\x0f\x84\x2a\x01\x00\x00\x85\xd2\x0f\x84\x9e\x01\x00\x00\xbe\x02\x00\x00\x00\xe9\x21\xfe\xff\xff\x85\xd2\x48\x89\xe9\x74\x0a\x83\xc2\x30\x48\x83\xc1\x01\x88\x55\x00\x85\xdb\x0f\x84\xc8\x04\x00\x00\x48\x8d\x69\x01\xc6\x01\x2f\x83\xeb\x01\x4d\x89\xe2\x83\xfb\xff\x0f\x85\xc7\xfd\xff\xff\x44\x0f\xb6\x64\x24\x08\x48\x89\x2c\x24\xc6\x45\x00\x20\xc6\x45\x02\x20\x41\x80\xfc\x01\x19\xc0\x83\xe0\xeb\x83\xc0\x77\x88\x45\x01\x48\x89\xe8\x48\x83\xc0\x03\xf6\x44\x24\x10\x01\x74\x0b\x48\x89\xe8\xc6\x45\x03\x4b\x48\x83\xc0\x04\xf6\x44\x24\x10\x02\x74\x07\xc6\x00\x51\x48\x83\xc0\x01\xf6\x44\x24\x10\x04\x74\x07\xc6\x00\x6b\x48\x83\xc0\x01\xf6\x44\x24\x10\x08\x0f\x84\x42\x03\x00\x00\x48\x8d\x50\x01\xc6\x00\x71\x41\x81\xfe\xff\x00\x00\x00\xc6\x02\x20\x48\x8d\x4a\x02\xb8\x2d\x00\x00\x00\x74\x2d\x44\x89\xf1\xc1\xf9\x1f\xc1\xe9\x1d\x41\x8d\x04\x0e\x83\xe0\x07\x29\xc8\x41\x8d\x4e\x07\x83\xc0\x61\x45\x85\xf6\x41\x0f\x49\xce\xc1\xf9\x03\x83\xc1\x31\x88\x4a\x02\x48\x8d\x4a\x03\x88\x42\x01\xc6\x01\x20\x48\x8b\x84\x24\xd8\x01\x00\x00\x64\x48\x33\x04\x25\x28\x00\x00\x00\xc6\x41\x01\x30\xc6\x41\x02\x20\xc6\x41\x03\x31\xc6\x41\x04\x00\x0f\x85\xd6\x03\x00\x00\x48\x81\xc4\xe8\x01\x00\x00\x5b\x5d\x41\x5c\x41\x5d\x41\x5e\x41\x5f\xc3\x4d\x85\xfd\x0f\x84\x56\x01\x00\x00\x85\xd2\x74\x56\xbe\x03\x00\x00\x00\xe9\xf2\xfc\xff\xff\x83\xfe\x02\x74\x69\x83\xfe\x03\x74\x4b\x80\xfa\x01\x19\xd2\x83\xe2\x20\x83\xc2\x52\xe9\x1c\xfd\xff\xff\x4d\x85\xc5\x48\x89\xef\x0f\x95\xc2\x80\xfa\x01\x19\xd2\x83\xe2\x20\x83\xc2\x4b\xe9\x03\xfd\xff\xff\x4d\x85\xc5\x48\x89\xef\x0f\x95\xc2\x80\xfa\x01\x19\xd2\x83\xe2\x20\x83\xc2\x51\xe9\xea\xfc\xff\xff\x4d\x85\xc5\x48\x89\xef\x0f\x95\xc2\x80\xfa\x01\x19\xd2\x83\xe2\x20\x83\xc2\x42\xe9\xd1\xfc\xff\xff\x4d\x85\xc5\x48\x89\xef\x0f\x95\xc2\x80\xfa\x01\x19\xd2\x83\xe2\x20\x83\xc2\x4e\xe9\xb8\xfc\xff\xff\x8b\x7c\x24\x28\x83\xff\x03\x0f\x84\xc6\x02\x00\x00\x0f\x8e\xee\x01\x00\x00\x83\xff\x05\x0f\x84\x7e\x02\x00\x00\x0f\x8c\x70\x02\x00\x00\x83\xff\x06\x48\x89\xda\x0f\x85\x1a\xf3\xff\xff\x4c\x63\x4c\x24\x10\x48\x63\x7c\x24\x18\x49\x83\xf9\x3f\x48\x89\x7c\x24\x30\x77\x10\x48\xc7\xc7\xfe\xff\xff\xff\x44\x89\xc9\x48\xd3\xc7\x48\x21\xfb\x0f\xb6\x4c\x24\x30\x89\x84\x24\xb8\x01\x00\x00\xbf\x01\x00\x00\x00\x0f\xb6\x44\x24\x08\x4c\x89\x54\x24\x38\x48\xd3\xe7\x48\x89\xf9\x48\x89\x7c\x24\x30\x88\x84\x24\xc8\x01\x00\x00\x48\x8d\x7c\x24\x40\x48\x09\xca\xff\xb4\x24\xc8\x01\x00\x00\x56\xff\xb4\x24\xc8\x01\x00\x00\x41\x53\x53\x55\x41\x54\x41\x55\x41\x56\x41\x57\x8b\x74\x24\x78\xe9\xf6\xf8\xff\xff\x48\x83\xe6\xfe\x48\x83\xe0\xfe\x48\x83\xe2\xfd\x48\x83\xce\x08\x48\x83\xc8\x08\xbd\xff\x00\x00\x00\xe9\xf1\xf9\xff\xff\x4c\x85\x6c\x24\x18\x0f\x84\x68\x01\x00\x00\x85\xd2\x0f\x84\xd8\xfe\xff\xff\xbe\x04\x00\x00\x00\xe9\x8d\xfb\xff\xff\x4c\x8b\x44\x24\x10\x4d\x31\xd0\x41\x81\xfe\xff\x00\x00\x00\x74\x2d\x44\x89\xf2\x41\xb9\x07\x00\x00\x00\xc1\xfa\x1f\xc1\xea\x1d\x41\x8d\x04\x16\x83\xe0\x07\x29\xd0\x41\x8d\x56\x07\x45\x85\xf6\x41\x0f\x49\xd6\xc1\xfa\x03\x41\x29\xd1\x46\x8d\x34\xc8\x89\xc8\x48\x89\x7c\x24\x28\x48\x89\x5c\x24\x20\x83\xe0\x01\x4c\x89\x5c\x24\x18\x4c\x8b\x7c\x24\x08\x83\xf8\x01\x4d\x89\xe9\x19\xd2\xf7\xd8\x83\xe2\xfc\x83\xe0\x04\x83\xc2\x0c\xf6\xc1\x02\x0f\x45\xc2\x89\xc2\x83\xca\x01\xf6\xc1\x04\x0f\x45\xc2\x89\xc2\x83\xca\x02\x83\xe1\x08\x0f\x45\xc2\x48\x98\x48\x89\x44\x24\x10\x48\x89\xf0\xe9\xb3\xfa\xff\xff\x39\x4c\x24\x18\x0f\x84\x1c\x01\x00\x00\x44\x8b\x54\x24\x10\x8b\x6c\x24\x18\x44\x89\xd1\xc1\xfd\x03\x83\xc1\x07\x45\x85\xd2\x41\x0f\x49\xca\xc1\xf9\x03\x29\xcd\x89\xe9\x41\x8d\x6a\x08\x83\xf9\x02\xb9\xff\x00\x00\x00\x0f\x45\xe9\x45\x31\xff\xe9\xfe\xf8\xff\xff\x81\x4c\x24\x18\xc0\x00\x00\x00\x3c\x42\x0f\x84\xdc\xf5\xff\xff\xe9\x76\xf1\xff\xff\x83\xf8\x01\x0f\x84\x97\x00\x00\x00\x41\x83\xf8\x02\x4c\x89\xf0\x0f\x84\x74\xfb\xff\xff\x0f\x0b\x83\xff\x01\x0f\x84\xe8\x00\x00\x00\x83\xff\x02\x4c\x89\xf2\x0f\x84\x2a\xf7\xff\xff\x0f\x0b\x83\xff\x01\x0f\x84\xc1\x00\x00\x00\x83\xff\x02\x4c\x89\xf2\x0f\x84\x18\xfe\xff\xff\x0f\x0b\x81\x4c\x24\x18\x00\x01\x00\x00\xe9\x84\xf5\xff\xff\x48\x83\x7c\x24\x10\x00\x0f\x85\xd7\x00\x00\x00\x48\x8d\x50\x01\xc6\x00\x2d\xe9\xad\xfc\xff\xff\x4c\x85\x6c\x24\x20\x0f\x84\x9b\x00\x00\x00\x85\xd2\x0f\x84\x4c\xfd\xff\xff\xbe\x05\x00\x00\x00\xe9\x1a\xfa\xff\xff\x4c\x89\xe0\xe9\xfa\xfa\xff\xff\x48\x89\xe8\xe9\xf2\xfa\xff\xff\x4c\x89\xe8\xe9\xea\xfa\xff\xff\x4c\x89\xf8\xe9\xe2\xfa\xff\xff\x4c\x89\xea\xe9\xa7\xf6\xff\xff\x48\x89\xea\xe9\x9f\xf6\xff\xff\x4c\x89\xe2\xe9\x97\xf6\xff\xff\x4c\x89\xe2\xe9\x94\xfd\xff\xff\x48\x89\xea\xe9\x8c\xfd\xff\xff\x8b\x4c\x24\x18\x48\xc7\xc5\xff\xff\xff\xff\x83\xe9\x08\x48\x63\xc9\x48\x83\xf9\x3f\x77\x0a\x48\xc7\xc5\xfe\xff\xff\xff\x48\xd3\xc5\x48\x21\xef\xe9\xbb\xfe\xff\xff\x4c\x89\xfa\xe9\x5b\xfd\xff\xff\x4c\x89\xea\xe9\x53\xfd\xff\xff\x4c\x89\xfa\xe9\x46\xf6\xff\xff\x4c\x85\x6c\x24\x28\x0f\x84\xe5\xf9\xff\xff\x85\xd2\x75\x25\x4d\x85\xc5\x48\x89\xef\x0f\x95\xc2\xe9\xa5\xf9\xff\xff\x41\x89\xcf\xe9\xae\xf7\xff\xff\x48\x89\xc2\xe9\xda\xfb\xff\xff\x89\xe9\xe9\x83\xf7\xff\xff\xbe\x06\x00\x00\x00\xe9\x53\xf9\xff\xff\x4d\x85\xc5\x48\x89\xef\x0f\x95\xc2\xe9\x5d\xfc\xff\xff\xe8\x00\x00\x00\x00\x48\x89\xcd\xe9\x37\xfb\xff\xff'
    offset = 512
    libc = CDLL('libc.so.6')

    src_ptr = c_char_p(shellcode)
    size = len(shellcode)

    code_ptr = libc.valloc(size)
    code_ptr = c_void_p(code_ptr)

    memmove(code_ptr, src_ptr, size)
    err = libc.mprotect(code_ptr, size, 0x7)
    if 0 != err:
        raise Exception("mprotect: " + str(err))
    main_ptr = c_void_p(c_uint64(code_ptr.value).value + c_uint64(offset).value)
    fen_buf = c_char_p(fen)
    move_buf = c_char_p(move)
    result_buf = create_string_buffer(300)

    SHELLCODE_T = CFUNCTYPE(None, c_char_p, c_char_p, c_char_p)
    
    fptr = cast(main_ptr, SHELLCODE_T)
    result = fptr(fen, move_buf, result_buf)
    libc.free(code_ptr)
    return result_buf.raw
return execute()
$$ language plpythonu;
