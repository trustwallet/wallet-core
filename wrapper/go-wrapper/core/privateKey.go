package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../libs/linux-amd64 -lTrustWalletCore -lwallet_core_rs -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWPrivateKey.h>
import "C"

import (
	"fmt"
	"unsafe"

	"github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/types"
)

type TWPrivateKey *C.struct_TWPrivateKey

func PrivateKeyCreateWithData(key []byte) (TWPrivateKey, error) {
	keyData := types.TWDataCreateWithGoBytes(key)
	defer C.TWDataDelete(keyData)
	privateKey := C.TWPrivateKeyCreateWithData(keyData)
	if privateKey == nil {
		return nil, fmt.Errorf("invalid private key")
	}
	return privateKey, nil
}

func PrivateKeyDelete(key TWPrivateKey) {
	C.TWPrivateKeyDelete(nil)
}

func PrivateKeySign(key TWPrivateKey, in unsafe.Pointer, curve CurveType) unsafe.Pointer {
	return C.TWPrivateKeySign(key, in, C.enum_TWCurve(curve))
}

func TWPrivateKeyGetPublicKeyEd25519Cardano(key TWPrivateKey) TWPublicKey {
	return C.TWPrivateKeyGetPublicKeyEd25519Cardano(key)
}

func TWPrivateKeyGetPublicKey(key TWPrivateKey, coinType CoinType) TWPublicKey {
	return C.TWPrivateKeyGetPublicKey(key, C.enum_TWCoinType(coinType))
}
