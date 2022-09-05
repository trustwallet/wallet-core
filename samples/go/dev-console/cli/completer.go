package cli

import (
	"github.com/c-bata/go-prompt"
	"strings"
)

var gCommands = []prompt.Suggest{
	{Text: "exit", Description: "Quit the CLI"},
	{Text: "create_wallet", Description: "Create a new wallet"},
	{Text: "load_wallet", Description: "Load an existing wallet"},
	{Text: "delete_wallet", Description: "Delete an existing wallet"},
	{Text: "address_all", Description: "Show the addresses of all accounts from the current wallet"},
	{Text: "help", Description: "Show the global help"},
}

type Completer struct {
}

func NewCompleter() (*Completer, error) {
	return &Completer{}, nil
}

func (c *Completer) argumentsCompleter(args []string) []prompt.Suggest {
	if len(args) <= 1 {
		return prompt.FilterContains(gCommands, args[0], true)
	}
	return []prompt.Suggest{}
}

func (c *Completer) Complete(d prompt.Document) []prompt.Suggest {
	if d.TextBeforeCursor() == "" {
		return []prompt.Suggest{}
	}
	args := strings.Split(d.TextBeforeCursor(), " ")
	return c.argumentsCompleter(args)
}
