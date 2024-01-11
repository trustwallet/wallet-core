// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHRP.h>

#include "../Coin.h"

enum TWBlockchain TWCoinTypeBlockchain(enum TWCoinType coin) {
    return TW::blockchain(coin);
}

enum TWPurpose TWCoinTypePurpose(enum TWCoinType coin) {
    return TW::purpose(coin);
}

enum TWCurve TWCoinTypeCurve(enum TWCoinType coin) {
    return TW::curve(coin);
}

enum TWHDVersion TWCoinTypeXpubVersion(enum TWCoinType coin) {
    return TW::xpubVersion(coin);
}

enum TWHDVersion TWCoinTypeXprvVersion(enum TWCoinType coin) {
    return TW::xprvVersion(coin);
}

bool TWCoinTypeValidate(enum TWCoinType coin, TWString *_Nonnull address) {
    return TW::validateAddress(coin, *reinterpret_cast<const std::string*>(address));
}

TWString *_Nonnull TWCoinTypeDerivationPath(enum TWCoinType coin) {
    const auto path = TW::derivationPath(coin);
    const auto string = path.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWString* TWCoinTypeDerivationPathWithDerivation(enum TWCoinType coin, enum TWDerivation derivation) {
    const auto path = TW::derivationPath(coin, derivation);
    const auto string = path.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWString *_Nonnull TWCoinTypeDeriveAddress(enum TWCoinType coin, struct TWPrivateKey *_Nonnull privateKey) {
    const auto string = TW::deriveAddress(coin, privateKey->impl);
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWString *_Nonnull TWCoinTypeDeriveAddressFromPublicKey(enum TWCoinType coin, struct TWPublicKey *_Nonnull publicKey) {
    const auto string = TW::deriveAddress(coin, publicKey->impl);
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWString *_Nonnull TWCoinTypeDeriveAddressFromPublicKeyAndDerivation(enum TWCoinType coin, struct TWPublicKey *_Nonnull publicKey, enum TWDerivation derivation) {
    const auto string = TW::deriveAddress(coin, publicKey->impl, derivation);
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

enum TWHRP TWCoinTypeHRP(enum TWCoinType coin) {
    return TW::hrp(coin);
}

uint8_t TWCoinTypeP2pkhPrefix(enum TWCoinType coin) {
    return TW::p2pkhPrefix(coin);
}

uint8_t TWCoinTypeP2shPrefix(enum TWCoinType coin) {
    return TW::p2shPrefix(coin);
}

uint8_t TWCoinTypeStaticPrefix(enum TWCoinType coin) {
    return TW::staticPrefix(coin);
}

TWString* _Nonnull TWCoinTypeChainId(enum TWCoinType coin) {
    return TWStringCreateWithUTF8Bytes(TW::chainId(coin));
}

uint32_t TWCoinTypeSlip44Id(enum TWCoinType coin) {
    return TW::slip44Id(coin);
}

enum TWPublicKeyType TWCoinTypePublicKeyType(enum TWCoinType coin) {
    return TW::publicKeyType(coin);
}

uint32_t TWCoinTypeSS58Prefix(enum TWCoinType coin) {
    return TW::ss58Prefix(coin);
}
