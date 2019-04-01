// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HDWallet.h"

#include "Base58.h"
#include "Bitcoin/Bech32Address.h"
#include "Bitcoin/CashAddress.h"
#include "Coin.h"
#include "Decred/Address.h"
#include "Ripple/Address.h"
#include "Zcash/TAddress.h"

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/bip39.h>
#include <TrezorCrypto/curves.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>

#include <array>

using namespace TW;

namespace {
HDNode getNode(const HDWallet& wallet, TWCurve curve, const DerivationPath& derivationPath);
HDNode getMasterNode(const HDWallet& wallet, TWCurve curve);

const char* curveName(TWCurve curve);
} // namespace

bool HDWallet::isValid(const std::string& mnemonic) {
    return mnemonic_check(mnemonic.c_str()) != 0;
}

HDWallet::HDWallet(int strength, const std::string& passphrase)
    : seed(), mnemonic(), passphrase(passphrase) {
    std::array<char, HDWallet::maxMnemomincSize> mnemonic_chars;
    mnemonic_generate(strength, mnemonic_chars.data());
    mnemonic_to_seed(mnemonic_chars.data(), passphrase.c_str(), seed.data(), nullptr);
    mnemonic = mnemonic_chars.data();
}

HDWallet::HDWallet(const std::string& mnemonic, const std::string& passphrase)
    : seed(), mnemonic(mnemonic), passphrase(passphrase) {
    mnemonic_to_seed(mnemonic.c_str(), passphrase.c_str(), seed.data(), nullptr);
}

HDWallet::HDWallet(const Data& data, const std::string& passphrase)
    : seed(), mnemonic(), passphrase(passphrase) {
    std::array<char, HDWallet::maxMnemomincSize> mnemonic_chars;
    mnemonic_from_data(data.data(), data.size(), mnemonic_chars.data());
    mnemonic_to_seed(mnemonic_chars.data(), passphrase.c_str(), seed.data(), nullptr);
    mnemonic = mnemonic_chars.data();
}

HDWallet::~HDWallet() {
    std::fill(seed.begin(), seed.end(), 0);
    std::fill(mnemonic.begin(), mnemonic.end(), 0);
    std::fill(passphrase.begin(), passphrase.end(), 0);
}

PrivateKey HDWallet::getKey(const DerivationPath& derivationPath) const {
    const auto curve = TWCoinTypeCurve(derivationPath.coin());
    auto node = getNode(*this, curve, derivationPath);
    auto data = Data(node.private_key, node.private_key + PrivateKey::size);
    return PrivateKey(data);
}

std::string HDWallet::deriveAddress(TWCoinType coin) const {
    const auto derivationPath = TW::derivationPath(coin);
    return TW::deriveAddress(coin, getKey(derivationPath));
}

std::string HDWallet::getExtendedPrivateKey(TWPurpose purpose, TWCoinType coin,
                                            TWHDVersion version) const {
    const auto curve = TWCoinTypeCurve(coin);
    auto derivationPath =
        TW::DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(coin, true)});
    auto node = getNode(*this, curve, derivationPath);
    std::array<char, HDWallet::maxExtendedKeySize> buffer = {0};
    auto fingerprint = hdnode_fingerprint(&node);
    hdnode_private_ckd(&node, 0x80000000);
    hdnode_serialize_private(&node, fingerprint, version, buffer.data(), HDWallet::maxExtendedKeySize);
    return buffer.data();
}

std::string HDWallet::getExtendedPublicKey(TWPurpose purpose, TWCoinType coin,
                                           TWHDVersion version) const {
    const auto curve = TWCoinTypeCurve(coin);
    auto derivationPath =
        TW::DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(coin, true)});
    auto node = getNode(*this, curve, derivationPath);
    std::array<char, HDWallet::maxExtendedKeySize> buffer = {0};
    auto fingerprint = hdnode_fingerprint(&node);
    hdnode_private_ckd(&node, 0x80000000);
    hdnode_fill_public_key(&node);
    hdnode_serialize_public(&node, fingerprint, version, buffer.data(), maxExtendedKeySize);
    return buffer.data();
}

PublicKey HDWallet::getPublicKeyFromExtended(const std::string& extended, TWCurve curve,
                                             enum TWHDVersion versionPublic,
                                             enum TWHDVersion versionPrivate, uint32_t change,
                                             uint32_t address) {
    auto node = HDNode{};
    uint32_t fingerprint = 0;

    hdnode_deserialize(extended.c_str(), versionPublic, versionPrivate, curveName(curve), &node,
                       &fingerprint);
    hdnode_public_ckd(&node, change);
    hdnode_public_ckd(&node, address);
    hdnode_fill_public_key(&node);

    return PublicKey(Data(node.public_key, node.public_key + 33));
}

namespace {
HDNode getNode(const HDWallet& wallet, TWCurve curve, const DerivationPath& derivationPath) {
    auto node = getMasterNode(wallet, curve);
    for (auto& index : derivationPath.indices) {
        hdnode_private_ckd(&node, index.derivationIndex());
    }
    return node;
}

HDNode getMasterNode(const HDWallet& wallet, TWCurve curve) {
    auto node = HDNode();
    hdnode_from_seed(wallet.seed.data(), HDWallet::seedSize, curveName(curve), &node);
    return node;
}

const char* curveName(TWCurve curve) {
    switch (curve) {
    case TWCurveSECP256k1:
        return SECP256K1_NAME;
    case TWCurveEd25519:
        return ED25519_NAME;
    case TWCurveNIST256p1:
        return NIST256P1_NAME;
    default:
        return "";
    }
}
} // namespace
