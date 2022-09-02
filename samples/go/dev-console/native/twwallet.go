package native

// #include <TrustWalletCore/TWHDWallet.h>
import "C"
import "errors"

type Wallet struct {
	wallet *C.struct_TWHDWallet
}

func NewWalletWithRandomMnemonic() *Wallet {
	return &Wallet{wallet: C.TWHDWalletCreate(256, NewTWString("").s)}
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

func (self *Wallet) IsValid() bool {
	return self.wallet != nil
}

func (self *Wallet) Delete() {
	C.TWHDWalletDelete(self.wallet)
}

func (self *Wallet) Seed() string {
	return TWDataHexString(C.TWHDWalletSeed(self.wallet))
}

func (self *Wallet) Mnemonic() string {
	str := TWString{s: C.TWHDWalletMnemonic(self.wallet)}
	defer str.Delete()
	return str.String()
}
