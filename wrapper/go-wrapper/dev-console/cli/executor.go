package cli

import (
	"fmt"
	"os"
	"strings"
)

func Executor(fullCommand string) {
	fullCommand = strings.TrimSpace(fullCommand)
	command := strings.Split(fullCommand, " ")
	switch command[0] {
	case "create_wallet":
		CreateWallet()
	case "load_wallet":
		LoadWallet()
	case "delete_wallet":
		DeleteWallet()
	case "address_all":
		DumpAllAddress()
	case "help":
		ShowGlobalHelp()
	case "exit":
		fmt.Println("Quitting the CLI")
		os.Exit(0)
	}
	return
}
