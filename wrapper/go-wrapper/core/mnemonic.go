package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../libs/linux-amd64 -lTrustWalletCore -lwallet_core_rs -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWMnemonic.h>
import "C"

import "github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/types"

func IsMnemonicValid(mn string) bool {
	str := types.TWStringCreateWithGoString(mn)
	defer C.TWStringDelete(str)
	return bool(C.TWMnemonicIsValid(str))
}
