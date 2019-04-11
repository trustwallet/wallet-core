// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../IOST/Account.h"

#include <TrustWalletCore/TWIOSTAccount.h>
#include <TrustWalletCore/TWIOSTProto.h>

using namespace TW;
using namespace TW::IOST;

bool TWIOSTAccountEqual(struct TWIOSTAccount* _Nonnull lhs, struct TWIOSTAccount* _Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWIOSTAccountIsValidString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Account::isValid(*s);
}

struct TWIOSTAccount* _Nullable TWIOSTAccountCreateWithString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);

    try {
        const auto account = Account(*s);
        return new TWIOSTAccount{std::move(account)};
    } catch (...) {
        return nullptr;
    }
}

bool TWIOSTAccountIsValidProto(TW_IOST_Proto_AccountInfo proto) {
    auto p = reinterpret_cast<const Proto::AccountInfo*>(proto);
    return Account::isValid(p->name());
}

struct TWIOSTAccount* _Nullable TWIOSTAccountCreateWithProto(TW_IOST_Proto_AccountInfo proto) {
    auto p = reinterpret_cast<const Proto::AccountInfo*>(proto);

    try {
        const auto account = Account(*p);
        return new TWIOSTAccount{std::move(account)};
    } catch (...) {
        return nullptr;
    }
}

void TWIOSTAccountDelete(struct TWIOSTAccount* _Nonnull account) {
    delete account;
}

TWString* _Nonnull TWIOSTAccountDescription(struct TWIOSTAccount* _Nonnull account) {
    const auto string = account->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
