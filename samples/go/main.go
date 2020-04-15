package main

// #cgo CFLAGS: -I../../include
// #cgo LDFLAGS: -L../../build -L../../build/trezor-crypto -lTrustWalletCore -lprotobuf -lTrezorCrypto -lc++ -lm
// #include <TrustWalletCore/TWHDWallet.h>
// #include <TrustWalletCore/TWString.h>
// #include <TrustWalletCore/TWData.h>
// #include <TrustWalletCore/TWPrivateKey.h>
// #include <TrustWalletCore/TWPublicKey.h>
import "C"

import "fmt"
import "unsafe"
import "encoding/hex"

// C.TWString -> Go string
func TWStringGoString(s unsafe.Pointer) string {
    return C.GoString(C.TWStringUTF8Bytes(s))
}

// Go string -> C.TWString
func TWStringCreateWithGoString(s string) unsafe.Pointer {
    cStr := C.CString(s)
    defer C.free(unsafe.Pointer(cStr))
    str := C.TWStringCreateWithUTF8Bytes(cStr)
    return str
}

// C.TWData -> Go byte[]
func TWDataGoBytes(d unsafe.Pointer) []byte {
    cBytes := C.TWDataBytes(d)
    cSize := C.TWDataSize(d)
    return C.GoBytes(unsafe.Pointer(cBytes), C.int(cSize))
}

// Go byte[] -> C.TWData
func TWDataCreateWithGoBytes(d []byte) unsafe.Pointer {
    cBytes := C.CBytes(d)
    defer C.free(unsafe.Pointer(cBytes))
    data := C.TWDataCreateWithBytes((*C.uchar)(cBytes), C.ulong(len(d)))
    return data
}

func main() {
    fmt.Println("==> calling wallet core from go")
    str := TWStringCreateWithGoString("confirm bleak useless tail chalk destroy horn step bulb genuine attract split")
    emtpy := TWStringCreateWithGoString("")
    defer C.TWStringDelete(str)
    defer C.TWStringDelete(emtpy)

    fmt.Println("<== mnemonic is valid: ", C.TWHDWalletIsValid(str))

    wallet := C.TWHDWalletCreateWithMnemonic(str, emtpy)
    defer C.TWHDWalletDelete(wallet)

    key := C.TWHDWalletGetKeyForCoin(wallet, C.TWCoinTypeBitcoin)
    keyData := C.TWPrivateKeyData(key)
    keyHex := hex.EncodeToString(TWDataGoBytes(keyData))
    fmt.Println("<== bitcoin private key: ", keyHex)

    pubKey, _ := hex.DecodeString("0288be7586c41a0498c1f931a0aaf08c15811ee2651a5fe0fa213167dcaba59ae8")
    pubKeyData := TWDataCreateWithGoBytes(pubKey)
    defer C.TWDataDelete(pubKeyData)

    fmt.Println("<== bitcoin public key is valid: ", C.TWPublicKeyIsValid(pubKeyData, C.TWPublicKeyTypeSECP256k1))

    address := C.TWHDWalletGetAddressForCoin(wallet, C.TWCoinTypeBitcoin)
    fmt.Println("<== bitcoin address: ", TWStringGoString(address))
}
