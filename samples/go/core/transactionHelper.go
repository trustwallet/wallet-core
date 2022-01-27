package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../build -L../../../build/trezor-crypto -lTrustWalletCore -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWTransactionHelper.h>
import "C"
import "tw/types"

func BuildInput(c CoinType, from, to string, amount string, asset string, memo string, chainId string) []byte {
	fromStr := types.TWStringCreateWithGoString(from)
	defer C.TWStringDelete(fromStr)
	toStr := types.TWStringCreateWithGoString(to)
	defer C.TWStringDelete(toStr)
	amountStr := types.TWStringCreateWithGoString(amount)
	defer C.TWStringDelete(amountStr)
	assetStr := types.TWStringCreateWithGoString(asset)
	defer C.TWStringDelete(assetStr)
	memoStr := types.TWStringCreateWithGoString(memo)
	defer C.TWStringDelete(memoStr)
	chainIdStr := types.TWStringCreateWithGoString(chainId)
	defer C.TWStringDelete(chainIdStr)

	result := C.TWTransactionHelperBuildInput(C.enum_TWCoinType(c), fromStr, toStr, amountStr, assetStr, memoStr, chainIdStr)
	defer C.TWDataDelete(result)
	return types.TWDataGoBytes(result)
}

func PreImageHash(c CoinType, txInputData []byte) []byte {
	input := types.TWDataCreateWithGoBytes(txInputData)
	defer C.TWDataDelete(input)

	result := C.TWTransactionHelperPreImageHash(C.enum_TWCoinType(c), input)
	defer C.TWDataDelete(result)
	return types.TWDataGoBytes(result)
}

func CompileWithSignature(c CoinType, txInputData []byte, signature []byte, publicKey []byte) []byte {
	input := types.TWDataCreateWithGoBytes(txInputData)
	defer C.TWDataDelete(input)
	sig := types.TWDataCreateWithGoBytes(signature)
	defer C.TWDataDelete(sig)
	pubkey := types.TWDataCreateWithGoBytes(publicKey)
	defer C.TWDataDelete(pubkey)

	result := C.TWTransactionHelperCompileWithSignature(C.enum_TWCoinType(c), input, sig, pubkey)
	defer C.TWDataDelete(result)
	return types.TWDataGoBytes(result)
}
