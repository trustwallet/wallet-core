// Copyright © 2019-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWWallet.h>

#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "../Keystore/Wallet.h"
#include "../Keystore/Account.h"
#include "../Keystore/StoredKey.h"

#include <string>

using namespace TW::Keystore;
using namespace TW;


struct TWWallet* _Nonnull TWWalletCreate(TWString* _Nonnull path, const struct TWStoredKey* _Nonnull key) {
    auto& pathString = *reinterpret_cast<const std::string*>(path);
    Wallet wallet = Wallet(pathString, key->impl);
    return new TWWallet{ wallet };
}

void TWWalletDelete(struct TWWallet* _Nonnull wallet) {
    delete wallet;
}

TWString* _Nonnull TWWalletIndentifier(struct TWWallet* _Nonnull wallet) {
    return TWStringCreateWithUTF8Bytes(wallet->impl.getIdentifier().c_str());
}

struct TWAnyAddress* _Nonnull TWWalletGetAccount(struct TWWallet* _Nonnull wallet, TWString* _Nonnull password, enum TWCoinType coin) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    Account account = wallet->impl.getAccount(passwordString, coin);
    return TWAnyAddressCreateWithString(TWStringCreateWithUTF8Bytes(account.address.c_str()), coin);
}

struct TWPrivateKey* _Nonnull TWWalletPrivateKey(struct TWWallet* _Nonnull wallet, const TWString* _Nonnull password, enum TWCoinType coin) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    auto privateKey = wallet->impl.privateKey(passwordString, coin);
    return new TWPrivateKey{ privateKey };
}
