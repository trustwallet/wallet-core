package cli

import (
	"dev-console/native"
	"dev-console/wallet"
	"github.com/kyokomi/emoji/v2"
)

func DumpAllAddress() {
	if wallet.GlobalWallet == nil || !wallet.GlobalWallet.Ks.IsLoaded() {
		emoji.Printf(":warning:No wallet loaded, use load_wallet or create_wallet first :warning:\n")
		return
	}
	nbWallets := wallet.GlobalWallet.Ks.AccountCount()
	var accounts []*native.Account
	for idx := int32(0); idx < nbWallets; idx++ {
		accounts = append(accounts, wallet.GlobalWallet.Ks.Account(idx))
	}
	native.ToTableAccounts(accounts)
	for _, account := range accounts {
		account.Delete()
	}
}
