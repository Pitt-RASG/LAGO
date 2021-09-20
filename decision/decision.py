from cffi import FFI

ffi = FFI()
ffi.cdef('''
struct API {

};
''')
@ffi.callback("
