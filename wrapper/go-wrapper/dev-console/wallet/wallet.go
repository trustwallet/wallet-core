package wallet

import (
	"dev-console/native"
	"errors"
	"github.com/kyokomi/emoji/v2"
	"log"
	"os"
	"path/filepath"
)

type Wallet struct {
	Ks         *native.StoredKey
	Password   string
	WalletName string
}

var GlobalWallet *Wallet = nil

func (self *Wallet) Dump() {
	emoji.Printf("Wallet Name: %s :white_check_mark:\n", self.WalletName)
	// Should we really dump password?
	emoji.Printf("Wallet Password: %s :white_check_mark:\n", self.Password)
	wallet := self.Ks.Wallet(self.Password)
	defer wallet.Delete()
	emoji.Printf("Wallet Mnemonic: %s :white_check_mark:\n", wallet.Mnemonic())
	emoji.Printf("Wallet Accounts count: %d :white_check_mark:\n", self.Ks.AccountCount())
	// Should we dump other infos?
}

func GetWalletDataDirectory() string {
	pwd, _ := os.Getwd()
	dir := filepath.Join(pwd, "data")
	if _, err := os.Stat(dir); errors.Is(err, os.ErrNotExist) {
		err := os.Mkdir(dir, os.ModePerm)
		if err != nil {
			log.Println(err)
		}
	}
	return dir
}
