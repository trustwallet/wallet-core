package cli

import (
	"github.com/olekukonko/tablewriter"
	"os"
)

const (
	createWalletHelp = `The create_wallet command create a new wallet with a given name and password. 
You can choose to restore or generate a seed.`
	createWalletUsage = `create_wallet`
	loadWalletHelp    = `The load_wallet command load the current wallet, the password will be prompted.`
	loadWalletUsage   = `load_wallet`
	deleteWalletHelp  = `The delete_wallet command delete the chosen wallet, password will be prompted.`
	deleteWalletUsage = `delete_wallet`
	addressAllHelp    = `The address_all command dump a table of all address accounts of the current loaded wallet.
Wallet need to be loaded with load_wallet before usage.`
	addressAllUsage = `address_all`
)

func ShowGlobalHelp() {
	data := [][]string{
		{"create_wallet", "", createWalletHelp, createWalletUsage},
		{"load_wallet", "", loadWalletHelp, loadWalletUsage},
		{"delete_wallet", "", deleteWalletHelp, deleteWalletUsage},
		{"address_all", "", addressAllHelp, addressAllUsage},
	}

	table := tablewriter.NewWriter(os.Stdout)
	table.SetAutoWrapText(false)
	table.SetHeader([]string{"Command", "Args", "Description", "Usage"})
	table.SetBorders(tablewriter.Border{Left: true, Top: false, Right: true, Bottom: false})
	table.SetCenterSeparator("|")
	table.AppendBulk(data) // Add Bulk Data
	table.Render()
}
