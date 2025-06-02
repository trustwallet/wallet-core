package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../libs/linux-amd64 -lTrustWalletCore -lwallet_core_rs -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWDataVector.h>
import "C"
import "github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/types"

type TWDataVector *C.struct_TWDataVector

// Go [][]byte -> C.TWDataVector
func TWDataVectorCreateWithGoBytes(d [][]byte) TWDataVector {
	vec := C.TWDataVectorCreate()
	for i := 0; i < len(d); i++ {
		elem := types.TWDataCreateWithGoBytes(d[i])
		C.TWDataVectorAdd(vec, elem)
	}
	return vec
}

// C.TWDataVector -> Go [][]byte
func TWDataVectorGoBytes(d *C.struct_TWDataVector) [][]byte {
	var vec [][]byte
	cSize := int(C.TWDataVectorSize(d))
	for i := 0; i < cSize; i++ {
		elemC := C.TWDataVectorGet(d, C.ulong(i))
		elemG := types.TWDataGoBytes(elemC)
		vec = append(vec, elemG)
	}
	return vec
}
