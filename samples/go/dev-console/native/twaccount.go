package native

// #include <TrustWalletCore/TWAccount.h>
import "C"

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
