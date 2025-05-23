package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../build -L../../../build/local/lib -lTrustWalletCore -lwallet_core_rs -lprotobuf -lstdc++ -lm
// #include <TrustWalletCore/Generated/TWMnemonic.h>
import "C"

import "tw/types"

func IsMnemonicValid(mn string) bool {
	str := types.TWStringCreateWithGoString(mn)
	defer C.TWStringDelete(str)
	return bool(C.TWMnemonicIsValid(str))
}
