package cli

import (
	"dev-console/native"
	"fmt"
	"github.com/manifoldco/promptui"
	"log"
	"os"
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

func CreateWallet() {
	walletName := ConfigureWalletName()
	wallet := ConfigureMnemonic()
	defer wallet.Delete()
	// use fake password for now, prompt later
	// TODO: Create a HDWallet with the mnemonic from ConfigureMnemonic
	// TODO: Create a StoredKey with this new HDWallet
	// TODO: prompt a passphrase for the wallet
	storedKey := native.NewStoredKeyFromHDWallet(wallet.Mnemonic(), walletName, "123", native.CoinTypeBitcoin)
	if storedKey != nil {
		pwd, _ := os.Getwd()
		res := storedKey.Store(filepath.Join(pwd, walletName+".json"))
		if res {
			fmt.Printf("Wallet %s successfully created\n", walletName)
		}
	} else {
		fmt.Println("Couldn't create the wallet")
	}
}
