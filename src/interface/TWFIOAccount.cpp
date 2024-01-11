// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
