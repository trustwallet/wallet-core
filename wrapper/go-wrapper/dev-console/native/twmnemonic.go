package native

// #include <TrustWalletCore/TWMnemonic.h>
import "C"

func IsMnemonicValid(mn string) bool {
	str := NewTWString(mn)
	defer str.Delete()
	return bool(C.TWMnemonicIsValid(str.s))
}
