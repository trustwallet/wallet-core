package wallet

import (
	"dev-console/native"
	"github.com/kyokomi/emoji/v2"
)

type Wallet struct {
	Ks         *native.StoredKey
	Password   string
	WalletName string
}

var GlobalWallet *Wallet = nil

func (self *Wallet) Dump() {
	emoji.Printf("Wallet Name: %s :white_check_mark:\n", self.WalletName)
	emoji.Printf("Wallet Password: %s :white_check_mark:\n", self.Password)
	wallet := self.Ks.Wallet(self.Password)
	defer wallet.Delete()
	emoji.Printf("Wallet Mnemonic: %s :white_check_mark:\n", wallet.Mnemonic())
}
