package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../build -L../../../build/trezor-crypto -lTrustWalletCore -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWMnemonic.h>
import "C"

import "tw/types"

func IsMnemonicValid(mn string) bool {
	str := types.TWStringCreateWithGoString(mn)
	defer C.TWStringDelete(str)
	return bool(C.TWMnemonicIsValid(str))
}
