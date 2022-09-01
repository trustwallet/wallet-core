package native

// #include <TrustWalletCore/TWStoredKey.h>
import "C"

type StoredKey struct {
	storedKey *C.struct_TWStoredKey
}
