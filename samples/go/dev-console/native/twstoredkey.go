package native

// #include <TrustWalletCore/TWStoredKey.h>
import "C"

type StoredKey struct {
	storedKey *C.struct_TWStoredKey
}

func NewStoredKeyFromHDWallet(mnemonic string, walletName string, password string, coinType CoinType) *StoredKey {
	mnemonicRaw := NewTWString(mnemonic)
	defer mnemonicRaw.Delete()
	walletNameRaw := NewTWString(walletName)
	defer walletNameRaw.Delete()
	passwordRaw := TWDataCreateWithGoBytes([]byte(password))
	sk := C.TWStoredKeyImportHDWallet(mnemonicRaw.s, walletNameRaw.s, passwordRaw, uint32(coinType))
	if sk != nil {
		return &StoredKey{storedKey: sk}
	}
	return nil
}

func (self *StoredKey) Store(path string) bool {
	pathRaw := NewTWString(path)
	defer pathRaw.Delete()
	return bool(C.TWStoredKeyStore(self.storedKey, pathRaw.s))
}

func (self *StoredKey) Wallet(password string) *Wallet {
	return &Wallet{wallet: C.TWStoredKeyWallet(self.storedKey, TWDataCreateWithGoBytes([]byte(password)))}
}
