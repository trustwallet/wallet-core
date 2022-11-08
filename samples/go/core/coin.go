package core

// #cgo CFLAGS: -I../../../include
// #cgo LDFLAGS: -L../../../build -L../../../build/local/lib -L../../../build/trezor-crypto -lTrustWalletCore -lwallet_core_rs -lprotobuf -lTrezorCrypto -lstdc++ -lm
// #include <TrustWalletCore/TWCoinType.h>
// #include <TrustWalletCore/TWCoinTypeConfiguration.h>
import "C"

import "tw/types"

type CoinType uint32

const (
	CoinTypeBitcoin  CoinType = C.TWCoinTypeBitcoin
	CoinTypeBinance  CoinType = C.TWCoinTypeBinance
	CoinTypeEthereum CoinType = C.TWCoinTypeEthereum
	CoinTypeTron     CoinType = C.TWCoinTypeTron
)

func (c CoinType) GetName() string {
	name := C.TWCoinTypeConfigurationGetName(C.enum_TWCoinType(c))
	defer C.TWStringDelete(name)
	return types.TWStringGoString(name)
}

func (c CoinType) Decimals() int {
	return int(C.TWCoinTypeConfigurationGetDecimals(C.enum_TWCoinType(c)))
}
