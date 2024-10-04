// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWTONWallet.h"
#include "TheOpenNetwork/TONWallet.h"
#include "rust/Wrapper.h"
#include "PublicKey.h"

using namespace TW;

bool TWTONWalletIsValidMnemonic(TWString* _Nonnull mnemonic, TWString* _Nullable passphrase) {
    const auto& mnemonicRef = *reinterpret_cast<const std::string*>(mnemonic);
    std::string passphraseObj;
    if (passphrase) {
        passphraseObj = std::string(TWStringUTF8Bytes(passphrase), TWStringSize(passphrase));
    }

    return TheOpenNetwork::TONWallet::isValidMnemonic(mnemonicRef, passphraseObj);
}

struct TWTONWallet* _Nullable TWTONWalletCreateWithMnemonic(TWString* _Nonnull mnemonic, TWString* _Nullable passphrase) {
    const auto& mnemonicRef = *reinterpret_cast<const std::string*>(mnemonic);
    std::string passphraseObj;
    if (passphrase) {
        passphraseObj = std::string(TWStringUTF8Bytes(passphrase), TWStringSize(passphrase));
    }

    const auto maybeWallet = TheOpenNetwork::TONWallet::createWithMnemonic(mnemonicRef, passphraseObj);
    if (!maybeWallet.has_value()) {
        return nullptr;
    }

    return new TWTONWallet { .impl = *maybeWallet };
}

void TWTONWalletDelete(struct TWTONWallet* _Nonnull wallet) {
    delete wallet;
}

struct TWPrivateKey* _Nonnull TWTONWalletGetKey(struct TWTONWallet* _Nonnull wallet) {
    const auto privateKey = wallet->impl.getKey();
    return new TWPrivateKey { .impl = privateKey };
}

TWString *_Nullable TWTONWalletBuildV4R2StateInit(struct TWPublicKey *_Nonnull publicKey, int32_t workchain, int32_t walletId) {
    auto keyType = static_cast<uint32_t>(TWPublicKeyKeyType(publicKey));
    auto* publicKeyRustRaw = Rust::tw_public_key_create_with_data(publicKey->impl.bytes.data(), publicKey->impl.bytes.size(), keyType);
    const auto publicKeyRust = Rust::wrapTWPublicKey(publicKeyRustRaw);

    Rust::TWStringWrapper stateInit = Rust::tw_ton_wallet_build_v4_r2_state_init(publicKeyRust.get(), workchain, walletId);
    if (!stateInit) {
        return nullptr;
    }
    return TWStringCreateWithUTF8Bytes(stateInit.c_str());
}
