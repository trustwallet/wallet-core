package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../build -L../../../build/local/lib -L../../../build/trezor-crypto -lTrustWalletCore -lwallet_core_rs -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWTransactionCompiler.h>
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

	result := C.TWTransactionCompilerBuildInput(C.enum_TWCoinType(c), fromStr, toStr, amountStr, assetStr, memoStr, chainIdStr)
	defer C.TWDataDelete(result)
	return types.TWDataGoBytes(result)
}

func PreImageHashes(c CoinType, txInputData []byte) []byte {
	input := types.TWDataCreateWithGoBytes(txInputData)
	defer C.TWDataDelete(input)

	result := C.TWTransactionCompilerPreImageHashes(C.enum_TWCoinType(c), input)
	defer C.TWDataDelete(result)
	return types.TWDataGoBytes(result)
}

func CompileWithSignatures(c CoinType, txInputData []byte, signatures [][]byte, publicKeyHashes [][]byte) []byte {
	input := types.TWDataCreateWithGoBytes(txInputData)
	defer C.TWDataDelete(input)

	sigs := TWDataVectorCreateWithGoBytes(signatures)
	defer C.TWDataVectorDelete(sigs)
	pubkeyhashes := TWDataVectorCreateWithGoBytes(publicKeyHashes)
	defer C.TWDataVectorDelete(pubkeyhashes)

	result := C.TWTransactionCompilerCompileWithSignatures(C.enum_TWCoinType(c), input, sigs, pubkeyhashes)
	defer C.TWDataDelete(result)
	return types.TWDataGoBytes(result)
}
