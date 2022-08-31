package main

import (
	"dev-console/native"
	"fmt"
)

func main() {
	s := native.NewTWString("Hey")
	defer s.Delete()
	println(s.String())
	fmt.Printf("%d\n", s.Size())
	fmt.Println(native.IsMnemonicValid("name dash bleak force moral disease shine response menu rescue more will"))
	wallet, _ := native.NewWalletWithMnemonic("name dash bleak force moral disease shine response menu rescue more will")
	defer wallet.Delete()
}
