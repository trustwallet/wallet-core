package cli

import (
	"dev-console/native"
	"dev-console/wallet"
	"fmt"
	"github.com/kyokomi/emoji/v2"
	"log"
	"os"
	"path/filepath"
)

func DeleteWallet() {
	wallets := listWallets()
	if len(wallets) == 0 {
		fmt.Println("No wallets found, use create_wallet instead.")
		return
	}
	walletName := chooseWallet(wallets)
	walletPath := filepath.Join(wallet.GetWalletDataDirectory(), walletName+".json")

	walletPassword := queryWalletPassword()
	storedKey := native.Load(walletPath)
	if storedKey.IsLoaded() {

		if hdWallet := storedKey.Wallet(walletPassword); hdWallet.IsValid() {
			e := os.Remove(walletPath)
			if e != nil {
				log.Fatal(e)
			}
			wallet.GlobalWallet = nil
			emoji.Printf("Wallet %s successfully deleted :white_check_mark:\n", walletName)
			defer hdWallet.Delete()
		} else {
			fmt.Println("Password from the wallet is incorrect")
			DeleteWallet()
		}
	} else {
		fmt.Println("Can't load the wallet.")
	}
}
