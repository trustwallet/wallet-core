package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../build -L../../../build/local/lib -L../../../build/trezor-crypto -lTrustWalletCore -lwallet_core_rs -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWCoinType.h>
// #include <TrustWalletCore/TWAnySigner.h>
import "C"

import (
	"tw/types"

	"google.golang.org/protobuf/proto"
)

func CreateSignedTx(inputData proto.Message, ct CoinType, outputData proto.Message) error {
	ibytes, _ := proto.Marshal(inputData)
	idata := types.TWDataCreateWithGoBytes(ibytes)
	defer C.TWDataDelete(idata)

	odata := C.TWAnySignerSign(idata, C.enum_TWCoinType(ct))
	defer C.TWDataDelete(odata)

	err := proto.Unmarshal(types.TWDataGoBytes(odata), outputData)
	if err != nil {
		return err
	}
	return nil
}
