create or replace function pf_apply_move (fen varchar, move varchar) returns varchar stable as $$
from ctypes import (CDLL, c_long, c_char_p, c_void_p, memmove, cast, Structure, CFUNCTYPE, c_uint64, c_int, create_string_buffer)

def execute():
    shellcode = b'\x83\xfe\x03\x0f\x84\x67\x01\x00\x00\x7e\x55\x83\xfe\x05\x0f\x84\x1c\x01\x00\x00\x0f\x8c\xb6\x00\x00\x00\x83\xfe\x06\x0f\x85\xa5\x00\x00\x00\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x30\x48\x89\x57\x28\x48\x89\x07\x48\x8b\x44\x24\x10\x48\x89\x47\x08\x48\x8b\x44\x24\x18\x48\x89\x47\x10\x48\x8b\x44\x24\x20\x48\x89\x47\x18\x48\x8b\x44\x24\x28\x48\x89\x47\x20\xeb\x49\x66\x0f\x1f\x44\x00\x00\x83\xfe\x01\x0f\x84\xa7\x00\x00\x00\x83\xfe\x02\x75\x5a\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x10\x48\x89\x57\x08\x48\x89\x07\x48\x8b\x44\x24\x18\x48\x89\x47\x10\x48\x8b\x44\x24\x20\x48\x89\x47\x18\x48\x8b\x44\x24\x28\x48\x89\x47\x20\x48\x8b\x44\x24\x30\x48\x89\x47\x28\x48\x8b\x44\x24\x38\x48\x89\x47\x30\x48\x8b\x44\x24\x40\x48\x89\x47\x38\x48\x8b\x44\x24\x48\x48\x89\x47\x40\x48\x89\xf8\xc3\x66\x0f\x1f\x44\x00\x00\x0f\x0b\x66\x0f\x1f\x44\x00\x00\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x20\x48\x89\x57\x18\x48\x89\x07\x48\x8b\x44\x24\x10\x48\x89\x47\x08\x48\x8b\x44\x24\x18\x48\x89\x47\x10\x48\x8b\x44\x24\x28\x48\x89\x47\x20\x48\x8b\x44\x24\x30\x48\x89\x47\x28\xeb\x9c\x66\x0f\x1f\x84\x00\x00\x00\x00\x00\x48\x8b\x44\x24\x10\x48\x89\x54\x24\x08\x48\x89\x17\x48\x89\x47\x08\x48\x8b\x44\x24\x18\x48\x89\x47\x10\xe9\x59\xff\xff\xff\x90\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x28\x48\x89\x57\x20\x48\x89\x07\x48\x8b\x44\x24\x10\x48\x89\x47\x08\x48\x8b\x44\x24\x18\x48\x89\x47\x10\x48\x8b\x44\x24\x20\x48\x89\x47\x18\x48\x8b\x44\x24\x30\x48\x89\x47\x28\xe9\x39\xff\xff\xff\x66\x0f\x1f\x44\x00\x00\x48\x8b\x44\x24\x08\x48\x89\x54\x24\x18\x48\x89\x57\x10\x48\x89\x07\x48\x8b\x44\x24\x10\x48\x89\x47\x08\xe9\xf9\xfe\xff\xff\x90\x49\x89\xca\x48\x63\x4c\x24\x08\x31\xc0\x48\x83\xf9\x3f\x77\x15\xb8\x01\x00\x00\x00\x48\xd3\xe0\x48\x89\xc1\xb8\x01\x00\x00\x00\x48\x85\xcf\x74\x0b\xc3\x66\x2e\x0f\x1f\x84\x00\x00\x00\x00\x00\x48\x85\xce\x74\x06\xb8\x02\x00\x00\x00\xc3\x48\x85\xca\x74\x06\xb8\x03\x00\x00\x00\xc3\x49\x85\xca\x74\x06\xb8\x04\x00\x00\x00\xc3\x49\x85\xc8\x74\x06\xb8\x05\x00\x00\x00\xc3\x4c\x21\xc9\x48\x83\xf9\x01\x19\xc0\xf7\xd0\x83\xe0\x06\xc3\x0f\x1f\x44\x00\x00\x41\x57\x45\x31\xdb\x45\x31\xff\x41\xb8\x01\x00\x00\x00\x41\x56\x45\x31\xf6\x41\x55\x45\x31\xed\x41\x54\x45\x31\xe4\x55\x31\xed\x53\x31\xdb\x48\x81\xec\xe8\x00\x00\x00\x64\x48\x8b\x04\x25\x28\x00\x00\x00\x48\x89\x84\x24\xd8\x00\x00\x00\x31\xc0\x48\x89\x14\x24\xba\x38\x00\x00\x00\x31\xc0\x0f\x1f\x84\x00\x00\x00\x00\x00\x44\x0f\xb6\x0f\x8d\x0c\x02\x48\x83\xc7\x01\x48\x63\xc9\x41\x80\xf9\x42\x0f\x84\xd8\x01\x00\x00\x7f\x56\x41\x80\xf9\x33\x74\x43\x0f\x8e\x0a\x01\x00\x00\x41\x80\xf9\x36\x74\x2e\x7f\x1a\x41\x80\xf9\x34\x74\x2c\x41\x80\xf9\x35\x74\x23\x66\x0f\x1f\x44\x00\x00\x0f\x0b\x66\x0f\x1f\x44\x00\x00\x41\x80\xf9\x37\x74\x09\x41\x80\xf9\x38\x75\xec\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\x83\xc0\x01\xeb\x92\x66\x90\x41\x80\xf9\x62\x0f\x84\x8d\x01\x00\x00\x7f\x3c\x41\x80\xf9\x50\x0f\x84\x32\x01\x00\x00\x0f\x8f\xec\x00\x00\x00\x41\x80\xf9\x4b\x74\x66\x41\x80\xf9\x4e\x75\xa8\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\xeb\x27\x0f\x1f\x80\x00\x00\x00\x00\x41\x80\xf9\x70\x0f\x84\x0d\x01\x00\x00\x0f\x8f\x98\x00\x00\x00\x41\x80\xf9\x6b\x74\x41\x41\x80\xf9\x6e\x0f\x85\x68\xff\xff\xff\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4d\x09\xce\xe9\x76\xff\xff\xff\x0f\x1f\x44\x00\x00\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4c\x09\xcd\xe9\x3f\xff\xff\xff\x66\x0f\x1f\x44\x00\x00\x41\x80\xf9\x2f\x0f\x84\xa6\x03\x00\x00\x0f\x8e\x28\x01\x00\x00\x41\x80\xf9\x31\x0f\x84\x1f\xff\xff\xff\x41\x80\xf9\x32\x0f\x84\x12\xff\xff\xff\x0f\x0b\x66\x2e\x0f\x1f\x84\x00\x00\x00\x00\x00\x41\x80\xf9\x71\x74\x33\x41\x80\xf9\x72\x0f\x84\xd9\x00\x00\x00\x0f\x0b\x66\x0f\x1f\x44\x00\x00\x41\x80\xf9\x51\x0f\x85\xa6\x00\x00\x00\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4d\x09\xcc\xe9\xb5\xfe\xff\xff\x0f\x1f\x40\x00\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4c\x09\xcb\xe9\x7f\xfe\xff\xff\x66\x0f\x1f\x44\x00\x00\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4d\x09\xcd\xe9\x47\xfe\xff\xff\x66\x0f\x1f\x44\x00\x00\x41\x80\xf9\x52\x0f\x85\x0e\xfe\xff\xff\x4d\x89\xc1\x41\xba\x00\x00\x00\x00\x49\xd3\xe1\x48\x83\xf9\x3f\x4d\x0f\x47\xca\x4d\x09\xcb\x4d\x89\xc1\x49\xd3\xe1\x48\x83\xf9\x3f\xb9\x00\x00\x00\x00\x4c\x0f\x47\xc9\x4d\x09\xcf\xe9\x05\xfe\xff\xff\x0f\x1f\x40\x00\x41\x80\xf9\x20\x0f\x85\xce\xfd\xff\xff\x48\x83\xc7\x02\x45\x31\xd2\x0f\xb6\x07\x3c\x20\x74\x30\x48\x83\xc7\x01\x3c\x51\x0f\x84\x34\x02\x00\x00\x0f\x8f\x16\x02\x00\x00\x3c\x2d\x74\xe3\x3c\x4b\x0f\x85\xa2\xfd\xff\xff\x0f\xb6\x07\x49\x83\xca\x01\x3c\x20\x75\xd7\x0f\x1f\x80\x00\x00\x00\x00\x0f\xbe\x56\x01\x0f\xbe\x06\x80\x7e\x04\x00\x8d\x84\xd0\x17\xfe\xff\xff\x0f\xbe\x56\x03\x89\x44\x24\x10\x0f\xbe\x46\x02\x8d\x84\xd0\x17\xfe\xff\xff\x89\x44\x24\x08\x74\x2d\x0f\xb6\x46\x05\x3c\x4e\x0f\x84\x32\x08\x00\x00\x0f\x8e\x96\x06\x00\x00\x3c\x51\x0f\x84\x17\x08\x00\x00\x83\x4c\x24\x08\x40\x3c\x52\x0f\x85\x3e\xfd\xff\xff\x66\x0f\x1f\x44\x00\x00\x8b\x44\x24\x10\x4c\x89\x54\x24\x20\x49\x89\xd9\x49\x89\xe8\x4c\x89\xe1\x4c\x89\xea\x4c\x89\xf6\x4c\x89\x94\x24\xd0\x00\x00\x00\x4c\x89\xff\x4c\x89\x9c\x24\xc0\x00\x00\x00\x4c\x89\xbc\x24\x90\x00\x00\x00\x4c\x89\xb4\x24\x98\x00\x00\x00\x4c\x89\xac\x24\xa0\x00\x00\x00\x4c\x89\xa4\x24\xa8\x00\x00\x00\x48\x89\xac\x24\xb0\x00\x00\x00\x48\x89\x9c\x24\xb8\x00\x00\x00\xc7\x84\x24\xc8\x00\x00\x00\xff\x00\x00\x00\x50\xe8\xcc\xfb\xff\xff\x8b\x74\x24\x10\x49\x89\xe8\x49\x89\xd9\x89\x44\x24\x1c\x4c\x89\xe1\x4c\x89\xea\x4c\x89\xff\x89\xf0\xc1\xf8\x06\x89\x44\x24\x20\x89\xf0\x4c\x89\xf6\x83\xe0\x3f\x89\x44\x24\x10\x89\x04\x24\xe8\x98\xfb\xff\xff\x5e\x85\xc0\x41\x89\xc0\x4c\x8b\x54\x24\x20\x0f\x84\x38\x01\x00\x00\x83\xf8\x03\x0f\x84\xe3\x05\x00\x00\x0f\x8e\xff\x06\x00\x00\x83\xf8\x05\x0f\x84\xa0\x07\x00\x00\x0f\x8c\xb2\x07\x00\x00\x83\xf8\x06\x0f\x85\x60\xfc\xff\xff\x0f\xb6\x4c\x24\x08\x48\xc7\xc0\xfe\xff\xff\xff\x4c\x89\x54\x24\x20\x44\x89\xc6\x48\x8d\x7c\x24\x40\xff\xb4\x24\xd0\x00\x00\x00\xff\xb4\x24\xd0\x00\x00\x00\xff\xb4\x24\xd0\x00\x00\x00\x48\xd3\xc0\xff\xb4\x24\xd0\x00\x00\x00\x48\x21\xd8\xff\xb4\x24\xd0\x00\x00\x00\x48\x89\xc2\xff\xb4\x24\xd0\x00\x00\x00\xff\xb4\x24\xd0\x00\x00\x00\xff\xb4\x24\xd0\x00\x00\x00\xff\xb4\x24\xd0\x00\x00\x00\xe8\x6a\xf9\xff\xff\x48\x8b\x94\x24\xc0\x00\x00\x00\x4c\x8b\xbc\x24\x88\x00\x00\x00\x4c\x8b\xb4\x24\x90\x00\x00\x00\x4c\x8b\xac\x24\x98\x00\x00\x00\x48\x89\x94\x24\x10\x01\x00\x00\x4c\x8b\xa4\x24\xa0\x00\x00\x00\x48\x8b\xac\x24\xa8\x00\x00\x00\x48\x8b\x9c\x24\xb0\x00\x00\x00\x4c\x8b\x9c\x24\xb8\x00\x00\x00\x48\x8b\xb4\x24\xc8\x00\x00\x00\x48\x83\xc4\x48\x4c\x8b\x54\x24\x20\xeb\x58\x0f\x1f\x80\x00\x00\x00\x00\x3c\x6b\x74\x24\x3c\x71\x0f\x85\x8c\xfb\xff\xff\x49\x83\xca\x08\xe9\xbc\xfd\xff\xff\x0f\x1f\x00\x49\x83\xca\x02\xe9\xb0\xfd\xff\xff\x0f\x1f\x80\x00\x00\x00\x00\x49\x83\xca\x04\xe9\xa0\xfd\xff\xff\x0f\x1f\x80\x00\x00\x00\x00\x83\xea\x08\x83\xfa\xf8\x0f\x85\x0a\xfb\xff\xff\xe9\x81\xfd\xff\xff\x4c\x89\xd6\xba\xff\x00\x00\x00\x8b\x4c\x24\x18\x85\xc9\x0f\x85\x97\x00\x00\x00\x8b\x7c\x24\x14\x83\xff\x03\x0f\x84\x83\x04\x00\x00\x0f\x8e\xc6\x05\x00\x00\x83\xff\x05\x0f\x84\x60\x06\x00\x00\x0f\x8c\x52\x06\x00\x00\x48\x89\xd8\x83\xff\x06\x0f\x85\x0d\xfb\xff\xff\x48\x63\x7c\x24\x10\x48\x89\xf9\x48\x89\x7c\x24\x20\x48\x63\x7c\x24\x08\x48\x83\xf9\x3f\x77\x12\x0f\xb6\x4c\x24\x20\x49\xc7\xc1\xfe\xff\xff\xff\x49\xd3\xc1\x4c\x21\xc8\x89\xf9\x41\xb9\x01\x00\x00\x00\x4c\x89\x54\x24\x38\x89\x94\x24\xc8\x00\x00\x00\x48\x8d\x7c\x24\x40\x49\xd3\xe1\x4c\x09\xc8\x56\xff\xb4\x24\xd0\x00\x00\x00\x48\x89\xc2\x41\x53\x53\x55\x41\x54\x41\x55\x41\x56\x41\x57\x8b\x74\x24\x5c\xe9\x92\x00\x00\x00\x8b\x7c\x24\x18\x83\xff\x03\x0f\x84\xf4\x03\x00\x00\x0f\x8e\x46\x05\x00\x00\x83\xff\x05\x0f\x84\xe9\x05\x00\x00\x0f\x8c\xdb\x05\x00\x00\x48\x89\xd8\x83\xff\x06\x0f\x85\x76\xfa\xff\xff\x48\x63\x7c\x24\x10\x48\x89\xf9\x48\x89\x7c\x24\x20\x48\x63\x7c\x24\x08\x48\x83\xf9\x3f\x77\x12\x0f\xb6\x4c\x24\x20\x49\xc7\xc1\xfe\xff\xff\xff\x49\xd3\xc1\x4c\x21\xcb\x89\x94\x24\xc8\x00\x00\x00\x89\xf9\x41\xb9\x01\x00\x00\x00\x4c\x89\x54\x24\x38\x48\x8d\x7c\x24\x40\x49\xd3\xe1\x56\x4c\x09\xc8\xff\xb4\x24\xd0\x00\x00\x00\x41\x53\x48\x89\xc2\x53\x55\x41\x54\x41\x55\x41\x56\x41\x57\x8b\x74\x24\x60\xe8\x7d\xf7\xff\xff\x48\x8b\xb4\x24\xa8\x00\x00\x00\x48\x8b\x9c\x24\xa0\x00\x00\x00\x48\x8b\x8c\x24\xb0\x00\x00\x00\x4c\x8b\xa4\x24\xc8\x00\x00\x00\x48\x8b\xbc\x24\x88\x00\x00\x00\x48\x89\xb4\x24\xf8\x00\x00\x00\x4c\x8b\x9c\x24\x90\x00\x00\x00\x48\x89\x74\x24\x78\x4c\x8b\xbc\x24\x98\x00\x00\x00\x48\x89\x9c\x24\xf0\x00\x00\x00\x48\x8b\x94\x24\xb8\x00\x00\x00\x48\x89\x5c\x24\x60\x4c\x89\xe6\x48\x8b\xac\x24\xc0\x00\x00\x00\x48\x89\xbc\x24\xd8\x00\x00\x00\x4c\x89\x9c\x24\xe0\x00\x00\x00\x4c\x89\xbc\x24\xe8\x00\x00\x00\x48\x89\x8c\x24\x00\x01\x00\x00\x48\x89\x94\x24\x08\x01\x00\x00\x48\x89\xac\x24\x10\x01\x00\x00\x4c\x89\xa4\x24\x18\x01\x00\x00\x48\x89\x4c\x24\x70\x48\x83\xc4\x48\x4c\x8b\x54\x24\x38\x83\x7c\x24\x14\x06\x0f\x84\xa7\x03\x00\x00\x83\x7c\x24\x14\x05\x0f\x94\xc1\x83\x7c\x24\x08\x06\x0f\x94\xc3\x20\xcb\x0f\x84\xf8\x04\x00\x00\x41\xf6\xc2\x01\x74\x13\x40\x80\xe7\x7f\x80\xe2\x7f\x48\x83\xe6\xfe\x48\x83\xcf\x20\x48\x83\xca\x20\x83\x7c\x24\x08\x02\x0f\x94\xc1\x20\xd9\x0f\x84\xd6\x04\x00\x00\x41\x83\xe2\x02\x0f\x85\x25\x03\x00\x00\xbb\xff\x00\x00\x00\x83\x7c\x24\x14\x01\x40\x0f\x94\xc5\x83\x7c\x24\x10\x07\x41\x0f\x94\xc4\x41\x84\xec\x74\x0f\x49\x89\xf4\x49\x83\xe4\xfe\x40\xf6\xc6\x01\x49\x0f\x45\xf4\x40\xf6\xc6\x02\x74\x18\x8b\x44\x24\x10\x49\x89\xf4\x85\xc0\x41\x0f\x94\xc5\x49\x83\xe4\xfd\x41\x84\xed\x49\x0f\x45\xf4\x41\x83\xf8\x01\x41\x0f\x94\xc0\x83\x7c\x24\x08\x3f\x75\x0e\x48\x89\xf5\x48\x83\xe5\xfb\x45\x84\xc0\x48\x0f\x45\xf5\x83\x7c\x24\x08\x38\x75\x0e\x48\x89\xf0\x48\x83\xe0\xf7\x45\x84\xc0\x48\x0f\x45\xf0\x48\x8b\x44\x24\x20\x49\x89\xf0\x49\x83\xe0\xfc\x84\xc9\x49\x0f\x45\xf0\x48\x83\xf8\x3f\x77\x0f\x49\xc7\xc0\xfe\xff\xff\xff\x89\xc1\x49\xd3\xc0\x4c\x21\xc2\x4c\x8b\x04\x24\x49\x09\xd1\xbd\x01\x00\x00\x00\x41\xba\x40\x00\x00\x00\x41\xbc\x07\x00\x00\x00\x48\x89\x74\x24\x08\x4d\x8d\x6a\xf8\x31\xc0\x4c\x89\xe9\xeb\x73\x66\x90\x49\x89\xee\x49\xd3\xe6\x4c\x85\xf7\x0f\x84\xa1\x01\x00\x00\x85\xc0\x0f\x84\x07\x04\x00\x00\xba\x01\x00\x00\x00\x49\x8d\x70\x01\x83\xc0\x30\x4d\x85\xf1\x41\x88\x00\x0f\x95\xc0\x83\xfa\x04\x0f\x84\xe5\x01\x00\x00\x0f\x8e\xa5\x01\x00\x00\x83\xfa\x05\x0f\x84\xbe\x01\x00\x00\x83\xfa\x06\x0f\x85\x9d\x01\x00\x00\x3c\x01\x19\xc0\x83\xe0\x20\x83\xc0\x50\x66\x0f\x1f\x84\x00\x00\x00\x00\x00\x88\x06\x4c\x8d\x46\x01\x48\x83\xc1\x01\x31\xc0\x4c\x39\xd1\x74\x12\x48\x83\xf9\x3f\x76\x89\x48\x83\xc1\x01\x83\xc0\x01\x4c\x39\xd1\x75\xee\x4c\x89\xc2\x85\xc0\x74\x0a\x83\xc0\x30\x48\x83\xc2\x01\x41\x88\x00\x45\x85\xe4\x0f\x84\x8f\x03\x00\x00\x4c\x8d\x42\x01\xc6\x02\x2f\x41\x83\xec\x01\x4d\x89\xea\x41\x83\xfc\xff\x0f\x85\x3e\xff\xff\xff\x48\x8b\x74\x24\x08\x4c\x89\xc0\x4c\x89\x04\x24\x41\xc6\x00\x20\x41\xc6\x40\x01\x77\x41\xc6\x40\x02\x20\x40\xf6\xc6\x01\x0f\x85\x4d\x02\x00\x00\x48\x83\xc0\x03\x40\xf6\xc6\x02\x74\x07\xc6\x00\x51\x48\x83\xc0\x01\x40\xf6\xc6\x04\x74\x07\xc6\x00\x6b\x48\x83\xc0\x01\x40\xf6\xc6\x08\x0f\x84\x36\x02\x00\x00\x48\x8d\x48\x01\xc6\x00\x71\xc6\x01\x20\x48\x8d\x51\x02\xb8\x2d\x00\x00\x00\x81\xfb\xff\x00\x00\x00\x74\x28\x89\xda\xc1\xfa\x1f\xc1\xea\x1d\x8d\x04\x13\x83\xe0\x07\x29\xd0\x8d\x53\x07\x83\xc0\x61\x85\xdb\x0f\x49\xd3\xc1\xfa\x03\x83\xc2\x31\x88\x51\x02\x48\x8d\x51\x03\x88\x41\x01\x48\x8b\x84\x24\xd8\x00\x00\x00\x64\x48\x33\x04\x25\x28\x00\x00\x00\xc6\x02\x20\xc6\x42\x01\x30\xc6\x42\x02\x20\xc6\x42\x03\x31\xc6\x42\x04\x00\x0f\x85\xa0\x02\x00\x00\x48\x81\xc4\xe8\x00\x00\x00\x5b\x5d\x41\x5c\x41\x5d\x41\x5e\x41\x5f\xc3\x81\x4c\x24\x08\xc0\x00\x00\x00\x3c\x42\x0f\x84\x75\xf9\xff\xff\x0f\x0b\x4c\x89\xe8\xe9\x96\xfb\xff\xff\x4c\x89\xe8\xe9\x25\xfc\xff\xff\x4c\x89\xeb\xe9\x33\xfa\xff\xff\x0f\x1f\x00\x4d\x85\xf3\x74\x12\x85\xc0\x0f\x84\x84\x00\x00\x00\xba\x02\x00\x00\x00\xe9\x55\xfe\xff\xff\x4d\x85\xf7\x0f\x84\xa7\x00\x00\x00\x85\xc0\x74\x55\xba\x03\x00\x00\x00\xe9\x3e\xfe\xff\xff\x66\x90\x83\xfa\x02\x74\x65\x83\xfa\x03\x74\x48\x3c\x01\x19\xc0\x83\xe0\x20\x83\xc0\x52\xe9\x67\xfe\xff\xff\x4d\x85\xf1\x4c\x89\xc6\x0f\x95\xc0\x3c\x01\x19\xc0\x83\xe0\x20\x83\xc0\x4b\xe9\x4f\xfe\xff\xff\x4d\x85\xf1\x4c\x89\xc6\x0f\x95\xc0\x3c\x01\x19\xc0\x83\xe0\x20\x83\xc0\x51\xe9\x37\xfe\xff\xff\x4d\x85\xf1\x4c\x89\xc6\x0f\x95\xc0\x3c\x01\x19\xc0\x83\xe0\x20\x83\xc0\x42\xe9\x1f\xfe\xff\xff\x4d\x85\xf1\x4c\x89\xc6\x0f\x95\xc0\x3c\x01\x19\xc0\x83\xe0\x20\x83\xc0\x4e\xe9\x07\xfe\xff\xff\x48\x83\xe7\xfe\x48\x83\xe2\xfe\x48\x83\xe6\xfd\x48\x83\xcf\x08\x48\x83\xca\x08\xbb\xff\x00\x00\x00\xe9\xc2\xfc\xff\xff\x4c\x85\x74\x24\x18\x0f\x84\xca\x00\x00\x00\x85\xc0\x74\x8b\xba\x04\x00\x00\x00\xe9\x8c\xfd\xff\xff\x39\x6c\x24\x08\x0f\x84\x17\x01\x00\x00\x8b\x44\x24\x10\x8b\x5c\x24\x08\x8d\x48\x07\xc1\xfb\x03\x85\xc0\x0f\x49\xc8\xc1\xf9\x03\x29\xcb\x8d\x48\x08\x83\xfb\x02\xbb\xff\x00\x00\x00\x0f\x44\xd9\x31\xc9\xe9\x72\xfc\xff\xff\x83\xf8\x01\x0f\x84\x99\x00\x00\x00\x4c\x89\xf3\x83\xf8\x02\x0f\x84\x04\xf9\xff\xff\x0f\x0b\x83\xff\x01\x0f\x84\xc2\x00\x00\x00\x4c\x89\xf0\x83\xff\x02\x0f\x84\x40\xfa\xff\xff\x0f\x0b\x83\xff\x01\x0f\x84\x93\x00\x00\x00\x4c\x89\xf0\x83\xff\x02\x0f\x84\xc0\xfa\xff\xff\x0f\x0b\x81\x4c\x24\x08\x00\x01\x00\x00\xe9\xef\xf7\xff\xff\x81\x4c\x24\x08\x80\x00\x00\x00\xe9\xe2\xf7\xff\xff\x4c\x89\xc0\x41\xc6\x40\x03\x4b\x48\x83\xc0\x04\xe9\xa6\xfd\xff\xff\x48\x85\xf6\x0f\x85\xd6\x00\x00\x00\x48\x8d\x48\x01\xc6\x00\x2d\xe9\xbc\xfd\xff\xff\x4c\x85\x74\x24\x30\x0f\x84\x85\x00\x00\x00\x85\xc0\x0f\x84\x9a\xfe\xff\xff\xba\x05\x00\x00\x00\xe9\xb3\xfc\xff\xff\x4c\x89\xfb\xe9\x6f\xf8\xff\xff\x48\x89\xeb\xe9\x67\xf8\xff\xff\x4c\x89\xe0\xe9\xb2\xf9\xff\xff\x48\x89\xe8\xe9\xaa\xf9\xff\xff\x4c\x89\xe3\xe9\x4f\xf8\xff\xff\x4c\x89\xf8\xe9\x31\xfa\xff\xff\x4c\x89\xe0\xe9\x29\xfa\xff\xff\x48\x89\xe8\xe9\x21\xfa\xff\xff\x4c\x89\xf8\xe9\x82\xf9\xff\xff\x8b\x44\x24\x08\x48\xc7\xc3\xff\xff\xff\xff\x8d\x48\xf8\x48\x63\xc9\x48\x83\xf9\x3f\x77\x0a\x48\xc7\xc3\xfe\xff\xff\xff\x48\xd3\xc3\x48\x21\x5c\x24\x28\xe9\xbe\xfe\xff\xff\x4c\x85\x74\x24\x28\x0f\x84\x90\xfc\xff\xff\x85\xc0\x75\x1c\x4d\x85\xf1\x4c\x89\xc6\x0f\x95\xc0\xe9\x54\xfc\xff\xff\x89\xcb\xe9\x1a\xfb\xff\xff\x89\xd9\xe9\x2d\xfb\xff\xff\xba\x06\x00\x00\x00\xe9\x0b\xfc\xff\xff\xe8\x00\x00\x00\x00\x48\x89\xc1\xe9\xea\xfc\xff\xff\x4d\x85\xf1\x4c\x89\xc6\x0f\x95\xc0\xe9\xbe\xfd\xff\xff\x49\x89\xd0\xe9\x70\xfc\xff\xff'
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
