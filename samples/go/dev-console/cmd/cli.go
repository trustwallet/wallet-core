package main

import (
	"dev-console/cli"
	prompt "github.com/c-bata/go-prompt"
)

func main() {
	completer, _ := cli.NewCompleter()
	p := prompt.New(
		cli.Executor,
		completer.Complete,
	)
	p.Run()
	//s := native.NewTWString("Hey")
	//defer s.Delete()
	//println(s.String())
	//fmt.Printf("%d\n", s.Size())
	//fmt.Println(native.IsMnemonicValid("name dash bleak force moral disease shine response menu rescue more will"))
	//wallet, _ := native.NewWalletWithMnemonic("name dash bleak force moral disease shine response menu rescue more will")
	//fmt.Printf("seed: [%s]\n", wallet.Seed())
	//fmt.Printf("mnemonic: [%s]\n", wallet.Mnemonic())
	//defer wallet.Delete()
}
