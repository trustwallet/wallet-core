package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../libs/linux-amd64 -lTrustWalletCore -lwallet_core_rs -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWCoinType.h>
// #include <TrustWalletCore/TWBitcoinScript.h>
// #include <TrustWalletCore/TWBitcoinSigHashType.h>
import "C"

import "github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/types"

const (
	BitcoinSigHashTypeAll          = C.TWBitcoinSigHashTypeAll
	BitcoinSigHashTypeNone         = C.TWBitcoinSigHashTypeNone
	BitcoinSigHashTypeSingle       = C.TWBitcoinSigHashTypeSingle
	BitcoinSigHashTypeFork         = C.TWBitcoinSigHashTypeFork
	BitcoinSigHashTypeForkBTG      = C.TWBitcoinSigHashTypeForkBTG
	BitcoinSigHashTypeAnyoneCanPay = C.TWBitcoinSigHashTypeAnyoneCanPay
)

func BitcoinScriptLockScriptForAddress(addr string, ct CoinType) []byte {
	address := types.TWStringCreateWithGoString(addr)
	defer C.TWStringDelete(address)

	script := C.TWBitcoinScriptLockScriptForAddress(address, C.enum_TWCoinType(ct))
	scriptData := C.TWBitcoinScriptData(script)
	defer C.TWBitcoinScriptDelete(script)
	defer C.TWDataDelete(scriptData)

	return types.TWDataGoBytes(scriptData)
}

func BitcoinScriptBuildPayToPublicKeyHash(hash []byte) []byte {
	hashData := types.TWDataCreateWithGoBytes(hash)
	defer C.TWDataDelete(hashData)

	script := C.TWBitcoinScriptBuildPayToPublicKeyHash(hashData)
	scriptData := C.TWBitcoinScriptData(script)
	defer C.TWBitcoinScriptDelete(script)
	defer C.TWDataDelete(scriptData)

	return types.TWDataGoBytes(scriptData)
}

func BitcoinScriptMatchPayToWitnessPublicKeyHash(script []byte) []byte {
	scriptData := types.TWDataCreateWithGoBytes(script)
	defer C.TWDataDelete(scriptData)
	scriptObj := C.TWBitcoinScriptCreateWithData(scriptData)
	defer C.TWBitcoinScriptDelete(scriptObj)

	hash := C.TWBitcoinScriptMatchPayToWitnessPublicKeyHash(scriptObj)
	defer C.TWDataDelete(hash)
	return types.TWDataGoBytes(hash)
}
