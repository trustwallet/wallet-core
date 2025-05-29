package cli

import (
	"dev-console/native"
	"dev-console/wallet"
	"fmt"
	"github.com/manifoldco/promptui"
	"log"
	"os"
	"path/filepath"
	"strings"
)

func listWallets() []string {
	files, err := os.ReadDir(wallet.GetWalletDataDirectory())
	if err != nil {
		log.Fatal(err)
	}

	var wallets []string
	for _, file := range files {
		wallets = append(wallets, strings.TrimSuffix(file.Name(), filepath.Ext(file.Name())))
	}
	return wallets
}

func chooseWallet(wallets []string) string {
	prompt := promptui.Select{
		Label: "Choose the wallet to load",
		Items: wallets,
	}
	_, result, _ := prompt.Run()
	return result
}

func queryWalletPassword() string {
	promptPassword := promptui.Prompt{
		Label: "Please, enter your wallet password",
	}
	walletPassword, err := promptPassword.Run()
	if err != nil {
		fmt.Printf("Prompt failed %v\n", err)
		walletPassword = queryWalletPassword()
	}

	if len(walletPassword) == 0 {
		fmt.Println("You're password cannot be empty, please try again")
		walletPassword = queryWalletPassword()
	}

	return walletPassword
}

func LoadWallet() {
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
		wallet.GlobalWallet = &wallet.Wallet{Ks: storedKey, Password: walletPassword, WalletName: walletName}
		if hdWallet := storedKey.Wallet(walletPassword); hdWallet.IsValid() {
			fmt.Printf("Wallet %s successfully loaded\n", walletName)
			defer hdWallet.Delete()
		} else {
			fmt.Println("Password from the wallet is incorrect")
			wallet.GlobalWallet = nil
			LoadWallet()
		}
	} else {
		fmt.Println("Can't load the wallet.")
	}
}
