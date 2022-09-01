package cli

import (
	"dev-console/native"
	"fmt"
	"github.com/manifoldco/promptui"
	"log"
)

func ConfigurePassPhrase() *native.Wallet {
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

	fmt.Printf("Your restored seed is [%s]\n", resultSeed)
	wallet, err := native.NewWalletWithMnemonic(resultSeed)
	if err != nil {
		log.Fatalf("Couldn't create the wallet: %v", err)
	}
	return wallet
}

func generateSeed() *native.Wallet {
	wallet := native.NewWalletWithRandomMnemonic()
	fmt.Printf("Your mnemonic is [%s]\n", wallet.Mnemonic())
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
	_ = ConfigureWalletName()
	wallet := ConfigurePassPhrase()
	defer wallet.Delete()
	// to remove
	fmt.Println(wallet.Mnemonic())
	fmt.Println(wallet.Seed())
	// TODO: Create a HDWallet with the mnemonic from ConfigurePassPhrase
	// TODO: Create a StoredKey with this new HDWallet
	// TODO: prompt a passphrase for the wallet
}
