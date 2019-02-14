// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HDWallet.h"

#include "Bitcoin/Bech32Address.h"
#include "Bitcoin/CashAddress.h"
#include "Zcash/TAddress.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/bip39.h>
#include <TrezorCrypto/curves.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>

using namespace TW;

namespace {
    HDNode getNode(const HDWallet& wallet, uint32_t purpose, uint32_t coin);
    HDNode getNode(const HDWallet& wallet, uint32_t purpose, uint32_t coin, uint32_t account, uint32_t change, uint32_t address);
    HDNode getMasterNode(const HDWallet& wallet);
}

bool HDWallet::isValid(const std::string& mnemonic) {
    return mnemonic_check(mnemonic.c_str()) != 0;
}

HDWallet::HDWallet(int strength, const std::string& passphrase) : seed(), mnemonic(), passphrase(passphrase) {
    char mnemonic_chars[HDWallet::maxMnemomincSize];
    mnemonic_generate(strength, mnemonic_chars);
    mnemonic_to_seed(mnemonic_chars, passphrase.c_str(), seed.data(), nullptr);
    mnemonic = mnemonic_chars;
}

HDWallet::HDWallet(const std::string& mnemonic, const std::string& passphrase) : seed(), mnemonic(mnemonic), passphrase(passphrase) {
    mnemonic_to_seed(mnemonic.c_str(), passphrase.c_str(), seed.data(), nullptr);
}

HDWallet::HDWallet(const Data& data, const std::string& passphrase) : seed(), mnemonic(), passphrase(passphrase) {
    char mnemonic_chars[maxMnemomincSize];
    mnemonic_from_data(data.data(), data.size(), mnemonic_chars);
    mnemonic_to_seed(mnemonic_chars, passphrase.c_str(), seed.data(), nullptr);
    mnemonic = mnemonic_chars;
}

HDWallet::~HDWallet() {
    std::fill(seed.begin(), seed.end(), 0);
    std::fill(mnemonic.begin(), mnemonic.end(), 0);
    std::fill(passphrase.begin(), passphrase.end(), 0);
}

PrivateKey HDWallet::getKey(TWPurpose purpose, TWCoinType coin, uint32_t account, uint32_t change, uint32_t address) const {
    auto node = getNode(*this, purpose, coin, account, change, address);
    auto data = Data(node.private_key, node.private_key  + PrivateKey::size);
    return PrivateKey(data);
}

std::string HDWallet::getExtendedPrivateKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const {
    auto node = getNode(*this, purpose, coin);
    char buffer[HDWallet::maxExtendedKeySize] = {0};
    auto fingerprint = hdnode_fingerprint(&node);
    hdnode_private_ckd(&node, 0x80000000);
    hdnode_serialize_private(&node, fingerprint, version, buffer, HDWallet::maxExtendedKeySize);
    return buffer;
}

std::string HDWallet::getExtendedPublicKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const {
    auto node = getNode(*this, purpose, coin);
    char buffer[HDWallet::maxExtendedKeySize] = {0};
    auto fingerprint = hdnode_fingerprint(&node);
    hdnode_private_ckd(&node, 0x80000000);
    hdnode_fill_public_key(&node);
    hdnode_serialize_public(&node, fingerprint, version, buffer, maxExtendedKeySize);
    return buffer;
}

PublicKey HDWallet::getPublicKeyFromExtended(const std::string& extended, enum TWHDVersion versionPublic, enum TWHDVersion versionPrivate, uint32_t change, uint32_t address) {
    auto node = HDNode{};
    uint32_t fingerprint = 0;

    hdnode_deserialize(extended.c_str(), versionPublic, versionPrivate, SECP256K1_NAME, &node, &fingerprint);
    hdnode_public_ckd(&node, change);
    hdnode_public_ckd(&node, address);
    hdnode_fill_public_key(&node);

    auto data = Data(node.public_key, node.public_key + PublicKey::compressedSize);
    return PublicKey(data);
}

std::optional<std::string> HDWallet::getAddressFromExtended(const std::string& extended, TWCoinType coinType, uint32_t change, uint32_t address) {
	uint8_t data[78];
	if (base58_decode_check(extended.c_str(), HASHER_SHA2D, data, sizeof(data)) != sizeof(data)) {
		return nullptr;
	}

	TWHDVersion version = (TWHDVersion) read_be(data);
    if (version != TWHDVersionXPUB && version != TWHDVersionYPUB && version != TWHDVersionLTUB && version != TWHDVersionZPUB && version != TWHDVersionMTUB) {
        // Not a public key
        return {};
    }

    auto publicKey = HDWallet::getPublicKeyFromExtended(extended, version, TWHDVersionNone, change, address);

    std::string string;
    switch (coinType) {
    case TWCoinTypeBitcoin: {
        auto address = Bitcoin::Bech32Address(reinterpret_cast<PublicKey&>(publicKey), 0, HRP_BITCOIN);
        string = address.encode();
    } break;
    case TWCoinTypeLitecoin: {
        auto address = Bitcoin::Bech32Address(reinterpret_cast<PublicKey&>(publicKey), 0, HRP_LITECOIN);
        string = address.encode();
    } break;
    case TWCoinTypeBitcoinCash: {
        auto address = Bitcoin::CashAddress(reinterpret_cast<PublicKey&>(publicKey));
        string = address.string();
    } break;
    case TWCoinTypeDash: {
        auto address = Bitcoin::Address(reinterpret_cast<PublicKey&>(publicKey), TWP2PKHPrefixDash);
        string = address.string();
    } break;
    case TWCoinTypeZcoin: {
        auto address = Bitcoin::Address(reinterpret_cast<PublicKey&>(publicKey), TWP2PKHPrefixZcoin);
        string = address.string();
    } break;
    case TWCoinTypeZcash: {
        auto address = Zcash::TAddress(reinterpret_cast<PublicKey&>(publicKey), TWP2PKHPrefixZcashT);
        string = address.string();
    } break;
    default:
        // Unknown coin
        return nullptr;
    }

    return string;
}

namespace {
    HDNode getNode(const HDWallet& wallet, uint32_t purpose, uint32_t coin) {
        auto node = getMasterNode(wallet);
        hdnode_private_ckd(&node, purpose | 0x80000000);
        hdnode_private_ckd(&node, coin | 0x80000000);
        return node;
    }

    HDNode getNode(const HDWallet& wallet, uint32_t purpose, uint32_t coin, uint32_t account, uint32_t change, uint32_t address) {
        auto node = getMasterNode(wallet);
        hdnode_private_ckd(&node, purpose | 0x80000000);
        hdnode_private_ckd(&node, coin | 0x80000000);
        hdnode_private_ckd(&node, account | 0x80000000);
        hdnode_private_ckd(&node, change);
        hdnode_private_ckd(&node, address);
        return node;
    }

    HDNode getMasterNode(const HDWallet& wallet) {
        auto node = HDNode();
        hdnode_from_seed(wallet.seed.data(), HDWallet::seedSize, SECP256K1_NAME, &node);
        return node;
    }
}
