// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Telegram/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWTelegramAddress.h>

using namespace TW;
using namespace TW::Telegram;

bool TWTelegramAddressEqual(struct TWTelegramAddress *_Nonnull lhs,
                          struct TWTelegramAddress *_Nonnull rhs)
{
    return lhs->impl == rhs->impl;
}

bool TWTelegramAddressIsValidString(TWString *_Nonnull string)
{
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWTelegramAddress *_Nullable TWTelegramAddressCreateWithString(TWString *_Nonnull string)
{
    auto s = reinterpret_cast<const std::string *>(string);
    try {
        const auto address = Address(*s);
        return new TWTelegramAddress{std::move(address)};
    } catch (...) {
        return nullptr;
    }
}

struct TWTelegramAddress *_Nonnull TWTelegramAddressCreateWithPublicKey(
    struct TWPublicKey *_Nonnull publicKey) {
    return new TWTelegramAddress{Address(publicKey->impl)};
}

void TWTelegramAddressDelete(struct TWTelegramAddress *_Nonnull address)
{
    delete address;
}

TWString *_Nonnull TWTelegramAddressDescription(struct TWTelegramAddress *_Nonnull address)
{
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
