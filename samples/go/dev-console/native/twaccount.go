package native

// #include <TrustWalletCore/TWAccount.h>
import "C"

type Account struct {
	account *C.struct_TWAccount
}

func (self *Account) Delete() {
	C.TWAccountDelete(self.account)
}
