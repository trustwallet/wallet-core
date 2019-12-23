package main

// #cgo CFLAGS: -I/wallet-core/include
// #cgo LDFLAGS: -L/wallet-core/build -L/wallet-core/build/trezor-crypto -lTrustWalletCore -lprotobuf -lTrezorCrypto -lc++ -lm
// #include <TrustWalletCore/TWHDWallet.h>
// #include <TrustWalletCore/TWString.h>
import "C"

import "fmt"

func main() {
    fmt.Println("==> calling wallet core from go")
    str := C.TWStringCreateWithUTF8Bytes(C.CString("confirm bleak useless tail chalk destroy horn step bulb genuine attract split"))
    defer C.TWStringDelete(str)
    valid := C.TWHDWalletIsValid(str)
    fmt.Println("<== mnemonic is valid: ", valid)
}
