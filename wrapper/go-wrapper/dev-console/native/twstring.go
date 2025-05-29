package native

// #include <TrustWalletCore/TWString.h>
import "C"

import (
	"unsafe"
)

type TWString struct {
	s unsafe.Pointer
}

func NewTWString(s string) TWString {
	cStr := C.CString(s)
	defer C.free(unsafe.Pointer(cStr))
	str := C.TWStringCreateWithUTF8Bytes(cStr)
	return TWString{s: str}
}

func (self TWString) Delete() {
	C.TWStringDelete(self.s)
}

func (self TWString) String() string {
	return C.GoString(C.TWStringUTF8Bytes(self.s))
}

func (self TWString) Size() int64 {
	return int64(C.size_t(C.TWStringSize(self.s)))
}
