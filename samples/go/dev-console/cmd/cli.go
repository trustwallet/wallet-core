package main

import (
	"dev-console/cli"
	"github.com/c-bata/go-prompt"
)

func main() {
	completer, _ := cli.NewCompleter()
	p := prompt.New(
		cli.Executor,
		completer.Complete,
	)
	p.Run()
}
