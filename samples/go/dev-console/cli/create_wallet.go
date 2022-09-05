package cli

import (
	"dev-console/native"
	"dev-console/wallet"
	"fmt"
	"github.com/kyokomi/emoji/v2"
	"github.com/manifoldco/promptui"
	"log"
	"path/filepath"
)

func ConfigureMnemonic() *native.Wallet {
	prompt := promptui.Select{
		Label: "Passphrase Configuration",
		Items: []string{"Generate a Seed", "Restore a Seed"},
	}
	_, result, _ := prompt.Run()

	if result == "Generate a Seed" {
		return generateSeed()
	} else if result == "Restore a Seed" {
		return restoreSeed()
	}
	return nil
}

func restoreSeed() *native.Wallet {
	promptRestoreSeed := promptui.Prompt{
		Label: "Please enter your seed",
	}
	resultSeed, err := promptRestoreSeed.Run()
	if err != nil {
		fmt.Printf("Prompt failed %v\n", err)
		return restoreSeed()
	}

	if len(resultSeed) == 0 {
		fmt.Println("You're custom seed cannot be empty, please try again")
		return restoreSeed()
	}

	if !native.IsMnemonicValid(resultSeed) {
		fmt.Println("You're seed is not a valid bip39 seed, please retry")
		return restoreSeed()
	}

	wallet, err := native.NewWalletWithMnemonic(resultSeed)
	if err != nil {
		log.Fatalf("Couldn't create the wallet: %v", err)
	}
	return wallet
}

func generateSeed() *native.Wallet {
	wallet := native.NewWalletWithRandomMnemonic()
	return wallet
}

func ConfigureWalletName() string {
	promptWalletName := promptui.Prompt{
		Label: "Please enter your wallet name",
	}
	walletName, err := promptWalletName.Run()
	if err != nil {
		fmt.Printf("Prompt failed %v\n", err)
		walletName = ConfigureWalletName()
	}

	if len(walletName) == 0 {
		fmt.Println("You're custom seed cannot be empty, please try again")
		walletName = ConfigureWalletName()
	}

	return walletName
}

func ConfigurePassword() string {
	promptPassword := promptui.Prompt{
		Label: "Please, choose a wallet password",
	}
	walletPassword, err := promptPassword.Run()
	if err != nil {
		fmt.Printf("Prompt failed %v\n", err)
		walletPassword = ConfigureWalletName()
	}

	if len(walletPassword) == 0 {
		fmt.Println("You're password cannot be empty, please try again")
		walletPassword = ConfigureWalletName()
	}

	// Do We really need to check if the password is strong? It's a dev console.
	return walletPassword
}

func CreateWallet() {
	walletName := ConfigureWalletName()
	freshWallet := ConfigureMnemonic()
	password := ConfigurePassword()
	defer freshWallet.Delete()
	storedKey := native.NewStoredKeyFromHDWallet(freshWallet.Mnemonic(), walletName, password, native.CoinTypeBitcoin)
	storedKey.AccountForCoin(password, native.CoinTypeBinance)
	storedKey.AccountForCoin(password, native.CoinTypeEthereum)
	if storedKey != nil {
		res := storedKey.Store(filepath.Join(wallet.GetWalletDataDirectory(), walletName+".json"))
		if res {
			_, _ = emoji.Println("Wallet successfully created :white_check_mark:")
			// the global wallet can be loaded on creation or with load_wallet `wallet_name` - afterwards we can query accounts and address
			// open to change the package name
			// I guess keep the password the time the app is open is OK, no need to query it again
			wallet.GlobalWallet = &wallet.Wallet{Ks: storedKey, Password: password, WalletName: walletName}
			wallet.GlobalWallet.Dump()
		}
	} else {
		fmt.Println("Couldn't create the wallet")
	}
}
