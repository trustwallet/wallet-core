// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HDWallet.h"

#include "Base58.h"
#include "BinaryCoding.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/SegwitAddress.h"
#include "Coin.h"
#include "ImmutableX/StarkKey.h"
#include "Mnemonic.h"
#include "memory/memzero_wrapper.h"

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPublicKeyType.h>

#include "Generated/HDNode.h"
#include <TrustWalletCore/Generated/TWHDNode.h>
#include "Generated/HDNodePublic.h"
#include <TrustWalletCore/Generated/TWHDNodePublic.h>

#include <array>
#include <cstring>

using namespace TW;

template <std::size_t seedSize>
HDWallet<seedSize>::HDWallet(const Data& seed) {
    std::copy_n(seed.begin(), seedSize, this->seed.begin());
}

template <std::size_t seedSize>
void HDWallet<seedSize>::updateSeedAndEntropy([[maybe_unused]] bool check) {
    assert(!check || Mnemonic::isValid(mnemonic)); // precondition

    auto seedData = Mnemonic::toSeed(mnemonic, passphrase);
    std::copy_n(seedData.begin(), seedSize, seed.begin());
    entropy = Mnemonic::toEntropy(mnemonic);
   
    assert(!check || entropy.size() > 10);
}

template <std::size_t seedSize>
HDWallet<seedSize>::HDWallet(int strength, const std::string& passphrase)
    : passphrase(passphrase) {
    mnemonic = Mnemonic::generate(strength);
    if (mnemonic.empty()) {
        throw std::invalid_argument("Invalid strength");
    }
    updateSeedAndEntropy();
}

template <std::size_t seedSize>
HDWallet<seedSize>::HDWallet(const std::string& mnemonic, const std::string& passphrase, const bool check)
    : mnemonic(mnemonic), passphrase(passphrase) {
    if (mnemonic.length() == 0 ||
        (check && !Mnemonic::isValid(mnemonic))) {
        throw std::invalid_argument("Invalid mnemonic");
    }
    updateSeedAndEntropy(check);
}

template <std::size_t seedSize>
HDWallet<seedSize>::HDWallet(const Data& entropy, const std::string& passphrase)
    : passphrase(passphrase) {
    mnemonic = Mnemonic::generateFromData(entropy);
    if (mnemonic.empty()) {
        throw std::invalid_argument("Invalid mnemonic data");
    }
    updateSeedAndEntropy();
}

template <std::size_t seedSize>
HDWallet<seedSize>::~HDWallet() {
    memzero(seed.data(), seed.size());
    memzero(mnemonic.data(), mnemonic.size());
    memzero(passphrase.data(), passphrase.size());
}

template <size_t seedSize>
static TWHDNode* getMasterNode(const HDWallet<seedSize>& wallet, TWCurve curve) {
    TWData* seedData = nullptr;
    if (curve == TWCurveED25519ExtendedCardano) {
        seedData = TWDataCreateWithBytes(wallet.getEntropy().data(), wallet.getEntropy().size());
    } else {
        seedData = TWDataCreateWithBytes(wallet.getSeed().data(), wallet.getSeed().size());
    }
    auto node = TWHDNodeCreateWithSeed(seedData, curve);
    TWDataDelete(seedData);
    return node;
}

template <size_t seedSize>
static TWHDNode* getNode(const HDWallet<seedSize>& wallet, TWCurve curve, const DerivationPath& derivationPath, TW::Hash::Hasher hasher) {
    auto node = getMasterNode<seedSize>(wallet, curve);
    if (node == nullptr) {
        return nullptr;
    }
    auto derivationPathString = TWStringCreateWithUTF8Bytes(derivationPath.string().c_str());
    auto childNode = TWHDNodeDeriveFromPath(node, derivationPathString, hasher);
    TWStringDelete(derivationPathString);
    TWHDNodeDelete(node);
    return childNode;
}

template <std::size_t seedSize>
PrivateKey HDWallet<seedSize>::getMasterKey(TWCurve curve) const {
    auto node = getMasterNode(*this, curve);  
    if (node == nullptr) {
        throw std::invalid_argument("Invalid master node");
    }
    auto privateKeyData = TWHDNodePrivateKeyData(node);
    TWHDNodeDelete(node);
    auto data = Data(TWDataBytes(privateKeyData), TWDataBytes(privateKeyData) + TWDataSize(privateKeyData));
    TWDataDelete(privateKeyData);
    return PrivateKey(data, curve);
}

template <std::size_t seedSize>
PrivateKey HDWallet<seedSize>::getKeyByCurve(TWCurve curve, const DerivationPath& derivationPath) const {
    auto curveToUse = curve == TWCurveStarkex ? TWCurveSECP256k1 : curve;
    auto node = getNode<seedSize>(*this, curveToUse, derivationPath, Hash::HasherSha256ripemd);
    if (node == nullptr) {
        throw std::invalid_argument("Invalid derivation path");
    }
    auto privateKeyData = TWHDNodePrivateKeyData(node);
    TWHDNodeDelete(node);
    auto data = Data(TWDataBytes(privateKeyData), TWDataBytes(privateKeyData) + TWDataSize(privateKeyData));
    TWDataDelete(privateKeyData);
    auto privateKey = PrivateKey(data, curveToUse);
    if (curve == TWCurveStarkex) {
        return ImmutableX::getPrivateKeyFromEthPrivKey(privateKey);
    } else {
        return privateKey;
    }
}

template <std::size_t seedSize>
PrivateKey HDWallet<seedSize>::getKey(TWCoinType coin, const DerivationPath& derivationPath) const {
    const auto curve = TWCoinTypeCurve(coin);
    return getKeyByCurve(curve, derivationPath);
}

template <std::size_t seedSize>
PrivateKey HDWallet<seedSize>::getKey(TWCoinType coin, TWDerivation derivation) const {
    const auto path = TW::derivationPath(coin, derivation);
    return getKey(coin, path);
}

template <std::size_t seedSize>
std::string HDWallet<seedSize>::getRootKey(TWCoinType coin, TWHDVersion version) const {
    const auto curve = TWCoinTypeCurve(coin);
    auto node = getMasterNode(*this, curve);
    if (node == nullptr) {
        return "Unsupported curve";
    }
    auto extendedPrivateKey = TWHDNodeExtendedPrivateKey(node, version, TW::base58Hasher(coin));
    TWHDNodeDelete(node);
    auto* bytes = TWStringUTF8Bytes(extendedPrivateKey);
    std::string result(bytes);
    TWStringDelete(extendedPrivateKey);
    return result;
}

template <std::size_t seedSize>
std::string HDWallet<seedSize>::deriveAddress(TWCoinType coin, TWDerivation derivation) const {
    const auto derivationPath = TW::derivationPath(coin, derivation);
    return TW::deriveAddress(coin, getKey(coin, derivationPath), derivation);
}

template <std::size_t seedSize>
std::string HDWallet<seedSize>::deriveAddress(TWCoinType coin) const {
    return deriveAddress(coin, TWDerivationDefault);
}

template <std::size_t seedSize>
std::string HDWallet<seedSize>::getExtendedPrivateKeyAccount(TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version, uint32_t account) const {
    if (version == TWHDVersionNone) {
        return "";
    }

    const auto curve = TWCoinTypeCurve(coin);
    const auto path = TW::derivationPath(coin, derivation);
    auto derivationPath = DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(path.coin(), true), DerivationPathIndex(account, true)});
    auto node = getNode(*this, curve, derivationPath, TW::publicKeyHasher(coin));
    if (node == nullptr) {
        return "Invalid derivation path";
    }
    auto extendedPrivateKey = TWHDNodeExtendedPrivateKey(node, version, TW::base58Hasher(coin));
    TWHDNodeDelete(node);
    auto* bytes = TWStringUTF8Bytes(extendedPrivateKey);
    std::string result(bytes);
    TWStringDelete(extendedPrivateKey);
    return result;
}

template <std::size_t seedSize>
std::string HDWallet<seedSize>::getExtendedPublicKeyAccount(TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version, uint32_t account) const {
    if (version == TWHDVersionNone) {
        return "";
    }

    const auto curve = TWCoinTypeCurve(coin);
    const auto path = TW::derivationPath(coin, derivation);
    auto derivationPath = DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(path.coin(), true), DerivationPathIndex(account, true)});
    auto node = getNode(*this, curve, derivationPath, TW::publicKeyHasher(coin));
    if (node == nullptr) {
        return "Invalid derivation path";
    }
    auto extendedPublicKey = TWHDNodeExtendedPublicKey(node, version, TW::base58Hasher(coin));
    TWHDNodeDelete(node);
    auto* bytes = TWStringUTF8Bytes(extendedPublicKey);
    std::string result(bytes);
    TWStringDelete(extendedPublicKey);
    return result;
}

template <std::size_t seedSize>
std::optional<PublicKey> HDWallet<seedSize>::getPublicKeyFromExtended(const std::string& extended, TWCoinType coin, const DerivationPath& path) {
    const auto curve = TW::curve(coin);
    auto extendedString = TWStringCreateWithUTF8Bytes(extended.c_str());
    auto node = TWHDNodePublicCreateWithExtendedPublicKey(extendedString, curve, TW::base58Hasher(coin));
    TWStringDelete(extendedString);
    if (node == nullptr) {
        return {};
    }
    auto childPath = DerivationPath({DerivationPathIndex(path.change(), false), DerivationPathIndex(path.address(), false)});
    auto childPathString = TWStringCreateWithUTF8Bytes(childPath.string().c_str());
    auto childNode = TWHDNodePublicDeriveFromPath(node, childPathString, TW::publicKeyHasher(coin));
    TWHDNodePublicDelete(node);
    TWStringDelete(childPathString);
    if (childNode == nullptr) {
        return {};
    }

    auto pubkeyData = TWHDNodePublicPublicKeyData(childNode);
    TWHDNodePublicDelete(childNode);
    auto data = Data(TWDataBytes(pubkeyData), TWDataBytes(pubkeyData) + TWDataSize(pubkeyData));
    TWDataDelete(pubkeyData);
    TWPublicKeyType keyType = TW::publicKeyType(coin);
    if (keyType == TWPublicKeyTypeSECP256k1Extended) {
        auto pubkey = PublicKey(data, TWPublicKeyTypeSECP256k1);
        return pubkey.extended();
    } else if (keyType == TWPublicKeyTypeNIST256p1Extended) {
        auto pubkey = PublicKey(data, TWPublicKeyTypeNIST256p1);
        return pubkey.extended();
    } else {
        return PublicKey(data, keyType);
    }
}

template <std::size_t seedSize>
std::optional<PrivateKey> HDWallet<seedSize>::getPrivateKeyFromExtended(const std::string& extended, TWCoinType coin, const DerivationPath& path) {
    const auto curve = TW::curve(coin);
    auto extendedString = TWStringCreateWithUTF8Bytes(extended.c_str());
    auto node = TWHDNodeCreateWithExtendedPrivateKey(extendedString, curve, TW::base58Hasher(coin));
    TWStringDelete(extendedString);
    if (node == nullptr) {
        return {};
    }
    auto childPath = DerivationPath({DerivationPathIndex(path.change(), false), DerivationPathIndex(path.address(), false)});
    auto childPathString = TWStringCreateWithUTF8Bytes(childPath.string().c_str());
    auto childNode = TWHDNodeDeriveFromPath(node, childPathString, TW::publicKeyHasher(coin));
    TWHDNodeDelete(node);
    TWStringDelete(childPathString);
    if (childNode == nullptr) {
        return {};
    }
    auto privkeyData = TWHDNodePrivateKeyData(childNode);
    TWHDNodeDelete(childNode);
    auto data = Data(TWDataBytes(privkeyData), TWDataBytes(privkeyData) + TWDataSize(privkeyData));
    TWDataDelete(privkeyData);
    return PrivateKey(data, curve);
}

template <std::size_t seedSize>
PrivateKey HDWallet<seedSize>::bip32DeriveRawSeed(TWCoinType coin, const Data& seed, const DerivationPath& path) {
    const auto curve = TWCoinTypeCurve(coin);
    auto wallet = HDWallet<seedSize>(seed);
    return wallet.getKeyByCurve(curve, path);
}

namespace TW {
template class HDWallet<32>;
template class HDWallet<64>;
} // namespace TW
