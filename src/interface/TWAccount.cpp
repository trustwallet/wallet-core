// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAccount.h>

#include "../Keystore/Account.h"

using namespace TW;

struct TWAccount* _Nonnull TWAccountCreate(TWString* _Nonnull address,
                                           enum TWCoinType coin,
                                           enum TWDerivation derivation, 
                                           TWString* _Nonnull derivationPath,
                                           TWString* _Nonnull publicKey,
                                           TWString* _Nonnull extendedPublicKey) {
    auto& addressString = *reinterpret_cast<const std::string*>(address);
    auto& derivationPathString = *reinterpret_cast<const std::string*>(derivationPath);
    auto& publicKeyString = *reinterpret_cast<const std::string*>(publicKey);
    auto& extendedPublicKeyString = *reinterpret_cast<const std::string*>(extendedPublicKey);
    const auto dp = DerivationPath(derivationPathString);
    return new TWAccount{
        Keystore::Account(addressString, coin, derivation, dp, publicKeyString, extendedPublicKeyString)
    };
}

void TWAccountDelete(struct TWAccount* _Nonnull account) {
    delete account;
}

TWString* _Nonnull TWAccountAddress(struct TWAccount* _Nonnull account) {
    return TWStringCreateWithUTF8Bytes(account->impl.address.c_str());
}

enum TWCoinType TWAccountCoin(struct TWAccount* _Nonnull account) {
    return account->impl.coin;
}

enum TWDerivation TWAccountDerivation(struct TWAccount* _Nonnull account) {
    return account->impl.derivation;
}

TWString* _Nonnull TWAccountDerivationPath(struct TWAccount* _Nonnull account) {
    return TWStringCreateWithUTF8Bytes(account->impl.derivationPath.string().c_str());
}

TWString* _Nonnull TWAccountPublicKey(struct TWAccount* _Nonnull account) {
    return TWStringCreateWithUTF8Bytes(account->impl.publicKey.c_str());
}

TWString* _Nonnull TWAccountExtendedPublicKey(struct TWAccount* _Nonnull account) {
    return TWStringCreateWithUTF8Bytes(account->impl.extendedPublicKey.c_str());
}
