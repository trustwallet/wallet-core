package native

// #include <TrustWalletCore/TWHDWallet.h>
import "C"
import "errors"

type Wallet struct {
	wallet *C.struct_TWHDWallet
}

func NewWalletWithMnemonic(mn string) (*Wallet, error) {
	if !IsMnemonicValid(mn) {
		return nil, errors.New("mnemonic is not valid")
	}
	str := NewTWString(mn)
	empty := NewTWString("")
	defer str.Delete()
	defer empty.Delete()

	tw := C.TWHDWalletCreateWithMnemonic(str.s, empty.s)
	return &Wallet{wallet: tw}, nil
}

func (self *Wallet) Delete() {
	C.TWHDWalletDelete(self.wallet)
}
