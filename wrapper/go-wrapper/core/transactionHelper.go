package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../libs/linux-amd64 -lTrustWalletCore -lwallet_core_rs -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWTransactionCompiler.h>
import "C"
import "github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/types"

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
