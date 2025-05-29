package native

// #include <TrustWalletCore/TWAccount.h>
import "C"
import (
	"github.com/olekukonko/tablewriter"
	"os"
)

type Account struct {
	account *C.struct_TWAccount
}

func (self *Account) Delete() {
	C.TWAccountDelete(self.account)
}

func (self *Account) Address() string {
	address := TWString{s: C.TWAccountAddress(self.account)}
	defer address.Delete()
	return address.String()
}

func (self *Account) CoinType() CoinType {
	return CoinType(C.TWAccountCoin(self.account))
}

func ToTableAccounts(accounts []*Account) {
	var data [][]string

	for _, account := range accounts {
		cur := []string{account.CoinType().GetName(), account.Address()}
		data = append(data, cur)
	}

	table := tablewriter.NewWriter(os.Stdout)
	table.SetAutoWrapText(false)
	table.SetHeader([]string{"Coin", "Address"})
	table.SetFooter([]string{"", ""})
	table.SetBorders(tablewriter.Border{Left: true, Top: false, Right: true, Bottom: false})
	table.SetCenterSeparator("|")
	table.AppendBulk(data) // Add Bulk Data
	table.Render()
}
