#include <TrustWalletCore/TWBravoAddress.h>

#include "../Data.h"
#include "../Bravo/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::Bravo;

bool TWBravoAddressEqual(struct TWBravoAddress *_Nonnull lhs, struct TWBravoAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWBravoAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWBravoAddress *_Nullable TWBravoAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);

    try {
        return new TWBravoAddress{ Address(*s) };
    } catch (...) {
        return nullptr;
    }
}

struct TWBravoAddress *_Nonnull TWBravoAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, TWBravoAddressType type) {
    return new TWBravoAddress{ Address(publicKey->impl, Address::prefixes[type]) };
}

struct TWBravoAddress *_Nullable TWBravoAddressCreateWithKeyHash(TWData *_Nonnull keyHash, TWBravoAddressType type) {
    auto d = reinterpret_cast<const Data *>(keyHash);
    try {
        return new TWBravoAddress{ Address(*d, Address::prefixes[type]) };
    } catch (...) {
        return nullptr;
    }
}

void TWBravoAddressDelete(struct TWBravoAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWBravoAddressDescription(struct TWBravoAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}