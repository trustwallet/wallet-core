#include <TrustWalletCore/TWEOSAddress.h>

#include "../Data.h"
#include "../EOS/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::EOS;

Type translateType(TWEOSAddressType type) {
    Type t;
    switch (type)
    {
        case TWEOSAddressTypeLegacy:
            t = Type::Legacy;
            break;

        case TWEOSAddressTypeModernK1:
            t = Type::ModernK1;
            break;

        case TWEOSAddressTypeModernR1:
            t = Type::ModernR1;
            break;
    }

    return t;
}

bool TWEOSAddressEqual(struct TWEOSAddress *_Nonnull lhs, struct TWEOSAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWEOSAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWEOSAddress *_Nullable TWEOSAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);

    try {
        return new TWEOSAddress{ Address(*s) };
    } catch (...) {
        return nullptr;
    }
}

struct TWEOSAddress *_Nonnull TWEOSAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, TWEOSAddressType type) {
    return new TWEOSAddress{ Address(publicKey->impl, translateType(type)) };
}

struct TWEOSAddress *_Nullable TWEOSAddressCreateWithKeyHash(TWData *_Nonnull keyHash, TWEOSAddressType type) {
    auto d = reinterpret_cast<const Data *>(keyHash);
    try {
        return new TWEOSAddress{ Address(*d, translateType(type)) };
    } catch (...) {
        return nullptr;
    }
}

void TWEOSAddressDelete(struct TWEOSAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWEOSAddressDescription(struct TWEOSAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}