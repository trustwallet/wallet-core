// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HDWallet.h"

#include "Base58.h"
#include "BinaryCoding.h"
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

uint32_t fingerprint(HDNode *node, Hash::Hasher hasher);
std::string serialize(const HDNode *node, uint32_t fingerprint, uint32_t version, bool use_public, Hash::Hasher hasher);
bool deserialize(const std::string& extended, TWCurve curve, Hash::Hasher hasher, uint32_t version_public, uint32_t version_private, HDNode *node);
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

std::string HDWallet::getExtendedPrivateKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const {
    const auto curve = TWCoinTypeCurve(coin);
    auto derivationPath = TW::DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(coin, true)});
    auto node = getNode(*this, curve, derivationPath);
    auto fingerprintValue = fingerprint(&node, publicKeyHasher(coin));
    hdnode_private_ckd(&node, 0x80000000);
    return serialize(&node, fingerprintValue, version, false, base58Hasher(coin));
}

std::string HDWallet::getExtendedPublicKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const {
    const auto curve = TWCoinTypeCurve(coin);
    auto derivationPath = TW::DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(coin, true)});
    auto node = getNode(*this, curve, derivationPath);
    auto fingerprintValue = fingerprint(&node, publicKeyHasher(coin));
    hdnode_private_ckd(&node, 0x80000000);
    hdnode_fill_public_key(&node);
    return serialize(&node, fingerprintValue, version, true, base58Hasher(coin));
}

PublicKey HDWallet::getPublicKeyFromExtended(const std::string &extended, TWCurve curve,
                                             Hash::Hasher hasher, enum TWHDVersion versionPublic,
                                             enum TWHDVersion versionPrivate, uint32_t change,
                                             uint32_t address) {
    auto node = HDNode{};
    assert(deserialize(extended, curve, hasher, versionPublic, versionPrivate, &node));
    hdnode_public_ckd(&node, change);
    hdnode_public_ckd(&node, address);
    hdnode_fill_public_key(&node);

    return PublicKey(Data(node.public_key, node.public_key + 33));
}

PrivateKey HDWallet::getPrivateKeyFromExtended(const std::string &extended, TWCurve curve,
                                               Hash::Hasher hasher, enum TWHDVersion versionPublic,
                                               enum TWHDVersion versionPrivate, uint32_t change,
                                               uint32_t address) {
    auto node = HDNode{};
    assert(deserialize(extended, curve, hasher, versionPublic, versionPrivate, &node));
    hdnode_private_ckd(&node, change);
    hdnode_private_ckd(&node, address);

    return PrivateKey(Data(node.private_key, node.private_key + 32));
}

namespace {

uint32_t fingerprint(HDNode *node, Hash::Hasher hasher) {
    hdnode_fill_public_key(node);
    auto digest = hasher(node->public_key, node->public_key + 33);
    return ((uint32_t) digest[0] << 24) + (digest[1] << 16) + (digest[2] << 8) + digest[3];
}

std::string serialize(const HDNode *node, uint32_t fingerprint, uint32_t version, bool use_public, Hash::Hasher hasher) {
    Data node_data;
    node_data.reserve(78);

    encode32BE(version, node_data);
    node_data.push_back(static_cast<uint8_t>(node->depth));
    encode32BE(fingerprint, node_data);
    encode32BE(node->child_num, node_data);
    node_data.insert(node_data.end(), node->chain_code, node->chain_code + 32);
    if (use_public) {
        node_data.insert(node_data.end(), node->public_key, node->public_key + 33);
    } else {
        node_data.push_back(0);
        node_data.insert(node_data.end(), node->private_key, node->private_key + 32);
    }

    return Base58::bitcoin.encodeCheck(node_data, hasher);
}

bool deserialize(const std::string& extended, TWCurve curve, Hash::Hasher hasher, uint32_t version_public, uint32_t version_private, HDNode *node) {
    memset(node, 0, sizeof(HDNode));
    node->curve = get_curve_by_name(curveName(curve));

    const auto node_data = Base58::bitcoin.decodeCheck(extended, hasher);
    if (node_data.size() != 78) {
        return false;
    }

    uint32_t version = decode32BE(node_data.data());
    if (version == version_public) {
        std::copy(node_data.begin() + 45, node_data.begin() + 45 + 33, node->public_key);
    } else if (version == version_private) { // private node
        if (node_data[45]) { // invalid data
            return false;
        }
        std::copy(node_data.begin() + 46, node_data.begin() + 46 + 32, node->private_key);
    } else {
        return false; // invalid version
    }
    node->depth = node_data[4];
    node->child_num = decode32BE(node_data.data() + 9);
    std::copy(node_data.begin() + 13, node_data.begin() + 13 + 32, node->chain_code);
    return true;
}

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
