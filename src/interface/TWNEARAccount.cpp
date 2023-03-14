// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNEARAccount.h>

#include "NEAR/Address.h"
#include "NEAR/Account.h"
#include "HexCoding.h"

#include <string>

using namespace TW;

struct TWNEARAccount {
    std::string description;
};

struct TWNEARAccount *_Nullable TWNEARAccountCreateWithString(TWString *_Nonnull string) {
    const auto& account = *reinterpret_cast<const std::string*>(string);
    if (TW::NEAR::Address::isValid(account)) {
        const auto addr = TW::NEAR::Address(account);
        return new TWNEARAccount{addr.string()};
    }
    if (TW::NEAR::Account::isValid(account)) {
        return new TWNEARAccount{account};
    }
    return nullptr;
}

void TWNEARAccountDelete(struct TWNEARAccount *_Nonnull account) {
    delete account;
}

TWString *_Nonnull TWNEARAccountDescription(struct TWNEARAccount *_Nonnull account) {
    return TWStringCreateWithUTF8Bytes(account->description.c_str());
}
