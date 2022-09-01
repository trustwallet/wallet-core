package cli

import (
	"dev-console/native"
	"fmt"
	"github.com/manifoldco/promptui"
)

func ConfigurePassPhrase() string {
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
	return ""
}

func restoreSeed() string {
	promptRestoreSeed := promptui.Prompt{
		Label: "Please enter your seed",
	}
	resultSeed, err := promptRestoreSeed.Run()
	if err != nil {
		fmt.Printf("Prompt failed %v\n", err)
		resultSeed = restoreSeed()
	}

	if len(resultSeed) == 0 {
		fmt.Println("You're custom seed cannot be empty, please try again")
		resultSeed = restoreSeed()
	}

	if !native.IsMnemonicValid(resultSeed) {
		fmt.Println("You're seed is not a valid bip39 seed, please retry")
		resultSeed = restoreSeed()
	}

	fmt.Printf("Your restored seed is [%s]\n", resultSeed)
	return resultSeed
}

func generateSeed() string {
	mnemonic := native.GenerateMnemonic()
	fmt.Printf("Your mnemonic is [%s]\n", mnemonic)
	return mnemonic
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
	_ = ConfigurePassPhrase()
	// TODO: Create a HDWallet with the mnemonic from ConfigurePassPhrase
	// TODO: Create a StoredKey with this new HDWallet
	// TODO: prompt a passphrase for the wallet
}
