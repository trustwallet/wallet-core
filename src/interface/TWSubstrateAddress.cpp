//
// Created by Fitz on 2022/1/26.
//

#include "../Substrate/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWSubstrateAddress.h>

using namespace TW;
using namespace TW::Substrate;

bool TWSubstrateAddressEqual(struct TWSubstrateAddress *_Nonnull lhs, struct TWSubstrateAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWSubstrateAddressIsValidString(TWString *_Nonnull string, int32_t network) {
    auto s = reinterpret_cast<const std::string*>(string);
    return TW::Substrate::Address::isValid(*s, network);
}

struct TWSubstrateAddress *_Nullable TWSubstrateAddressCreateWithString(TWString *_Nonnull string, int32_t network) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = TW::Substrate::Address(*s, network);
        return new TWSubstrateAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWSubstrateAddress *_Nonnull TWSubstrateAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, int32_t network) {
    return new TWSubstrateAddress{ TW::Substrate::Address(publicKey->impl, network) };
}

void TWSubstrateAddressDelete(struct TWSubstrateAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWSubstrateAddressDescription(struct TWSubstrateAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}