//
// Created by Fitz on 2021/7/9.
//

#include "../Kusama/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWKusamaAddress.h>

using namespace TW;
using namespace TW::Kusama;

bool TWKusamaAddressEqual(struct TWKusamaAddress *_Nonnull lhs, struct TWKusamaAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWKusamaAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWKusamaAddress *_Nullable TWKusamaAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWKusamaAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWKusamaAddress *_Nonnull TWKusamaAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWKusamaAddress{ Address(publicKey->impl) };
}

void TWKusamaAddressDelete(struct TWKusamaAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWKusamaAddressDescription(struct TWKusamaAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
