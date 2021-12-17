package main

import (
	"fmt"
	"tw/core"
)

func main() {
	mn := "confirm bleak useless tail chalk destroy horn step bulb genuine attract split"

	bw, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeBitcoin)
	if err != nil {
		panic(err)
	}
	printWallet(bw)

	ew, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeEthereum)
	if err != nil {
		panic(err)
	}
	printWallet(ew)

	tw, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeTron)
	if err != nil {
		panic(err)
	}
	printWallet(tw)
}

func printWallet(w *core.Wallet) {
	fmt.Printf("%s wallet: \n", w.CoinType.GetName())
	fmt.Printf("\t address: %s \n", w.Address)
	fmt.Printf("\t pri key: %s \n", w.PriKey)
	fmt.Printf("\t pub key: %s \n", w.PubKey)
	fmt.Println("")
}
