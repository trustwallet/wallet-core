package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../build -L../../../build/trezor-crypto -lTrustWalletCore -lprotobuf -lTrezorCrypto -lc++ -lm
// #include <TrustWalletCore/TWCoinType.h>
// #include <TrustWalletCore/TWBitcoinScript.h>
// #include <TrustWalletCore/TWBitcoinSigHashType.h>
import "C"

import "tw/types"

const (
	BitcoinSigHashTypeAll          = C.TWBitcoinSigHashTypeAll
	BitcoinSigHashTypeNone         = C.TWBitcoinSigHashTypeNone
	BitcoinSigHashTypeSingle       = C.TWBitcoinSigHashTypeSingle
	BitcoinSigHashTypeFork         = C.TWBitcoinSigHashTypeFork
	BitcoinSigHashTypeForkBTG      = C.TWBitcoinSigHashTypeForkBTG
	BitcoinSigHashTypeAnyoneCanPay = C.TWBitcoinSigHashTypeAnyoneCanPay
)

func BitcoinLockScriptForAddress(addr string, ct CoinType) []byte {
	address := types.TWStringCreateWithGoString(addr)
	defer C.TWStringDelete(address)

	script := C.TWBitcoinScriptLockScriptForAddress(address, C.enum_TWCoinType(ct))
	scriptData := C.TWBitcoinScriptData(script)
	defer C.TWBitcoinScriptDelete(script)
	defer C.TWDataDelete(scriptData)

	return types.TWDataGoBytes(scriptData)
}
