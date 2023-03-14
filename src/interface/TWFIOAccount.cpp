// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFIOAccount.h>

#include "FIO/Actor.h"
#include "FIO/Address.h"

#include <string>

using namespace TW;

struct TWFIOAccount {
    std::string description;
};

struct TWFIOAccount *_Nullable TWFIOAccountCreateWithString(TWString *_Nonnull string) {
    const auto& account = *reinterpret_cast<const std::string*>(string);
    if (FIO::Address::isValid(account)) {
        const auto addr = FIO::Address(account);
        return new TWFIOAccount{FIO::Actor::actor(addr)};
    }
    if (FIO::Actor::validate(account)) {
        return new TWFIOAccount{account};
    }
    return nullptr;
}

void TWFIOAccountDelete(struct TWFIOAccount *_Nonnull account) {
    delete account;
}

TWString *_Nonnull TWFIOAccountDescription(struct TWFIOAccount *_Nonnull account) {
    return TWStringCreateWithUTF8Bytes(account->description.c_str());
}
