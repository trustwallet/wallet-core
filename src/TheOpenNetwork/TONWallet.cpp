// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TONWallet.h"
#include "Coin.h"

namespace TW::TheOpenNetwork {

bool TONWallet::isValidMnemonic(const std::string& mnemonic, const MaybePassphrase& passphrase) {
    const Rust::TWStringWrapper mnemonicRust = mnemonic;

    if (passphrase.has_value()) {
        const Rust::TWStringWrapper passphraseRust = passphrase.value();
        return Rust::tw_ton_wallet_is_valid_mnemonic(mnemonicRust.get(), passphraseRust.get());
    } else {
        return Rust::tw_ton_wallet_is_valid_mnemonic(mnemonicRust.get(), nullptr);
    }
}

MaybeTONWallet TONWallet::createWithMnemonic(const std::string& mnemonic, const MaybePassphrase& passphrase) {
    const Rust::TWStringWrapper mnemonicRust = mnemonic;

    Rust::TWTONWallet* walletPtr;
    if (passphrase.has_value()) {
        const Rust::TWStringWrapper passphraseRust = passphrase.value();
        walletPtr = Rust::tw_ton_wallet_create_with_mnemonic(mnemonicRust.get(), passphraseRust.get());
    } else {
        walletPtr = Rust::tw_ton_wallet_create_with_mnemonic(mnemonicRust.get(), nullptr);
    }

    if (!walletPtr) {
        return std::nullopt;
    }

    return TONWallet(TONWalletPtr(walletPtr, Rust::tw_ton_wallet_delete));
}

PrivateKey TONWallet::getKey(TWCoinType coin, TWDerivation derivation) const {
    if (coin != TWCoinTypeTON || derivation != TWDerivationDefault) {
        throw std::invalid_argument("'TONWallet' supports TON coin and Default derivation only");
    }

    const auto privateKeyRust = wrapTWPrivateKey(Rust::tw_ton_wallet_get_key(impl.get()));
    const Rust::TWDataWrapper privateKeyBytes = Rust::tw_private_key_data(privateKeyRust.get());
    return PrivateKey(privateKeyBytes.toDataOrDefault());
}

std::string TONWallet::deriveAddress(TWCoinType coin, TWDerivation derivation) const {
    const auto key = getKey(coin, derivation);
    return TW::deriveAddress(coin, key, derivation);
}

} // namespace TW::TheOpenNetwork
