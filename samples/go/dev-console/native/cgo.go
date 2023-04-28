package native

// #cgo CFLAGS: -I packaged/include
// #cgo LDFLAGS: -lTrustWalletCore -lstdc++ -lm -lprotobuf -lwallet_core_rs -lTrezorCrypto
//
//
// #cgo LDFLAGS: -Wl,-rpath,${SRCDIR}/packaged/lib -L${SRCDIR}/packaged/lib
//
import "C"

import (
	_ "dev-console/native/packaged/include"
	_ "dev-console/native/packaged/lib"
)
