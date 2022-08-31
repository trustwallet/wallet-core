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
	return TWString{s: TWStringCreateWithGoString(s)}
}

func (self TWString) Delete() {
	TWStringDelete(self.s)
}

func (self TWString) String() string {
	return TWStringGoString(self.s)
}

// C.TWString -> Go string
func TWStringGoString(s unsafe.Pointer) string {
	return C.GoString(C.TWStringUTF8Bytes(s))
}

// Go string -> C.TWString
func TWStringCreateWithGoString(s string) unsafe.Pointer {
	cStr := C.CString(s)
	defer C.free(unsafe.Pointer(cStr))
	str := C.TWStringCreateWithUTF8Bytes(cStr)
	return str
}

func TWStringDelete(s unsafe.Pointer) {
	C.TWStringDelete(s)
}
