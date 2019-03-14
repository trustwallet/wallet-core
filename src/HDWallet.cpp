// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HDWallet.h"

#include "Coin.h"
#include "Bitcoin/Bech32Address.h"
#include "Bitcoin/CashAddress.h"
#include "Zcash/TAddress.h"
#include "Ripple/Address.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/bip39.h>
#include <TrezorCrypto/curves.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>

using namespace TW;

namespace {
    HDNode getNode(const HDWallet& wallet, TWCurve curve, const DerivationPath& derivationPath);
    HDNode getMasterNode(const HDWallet& wallet, TWCurve curve);

    const char* curveName(TWCurve curve);
}

bool HDWallet::isValid(const std::string& mnemonic) {
    return mnemonic_check(mnemonic.c_str()) != 0;
}

HDWallet::HDWallet(int strength, const std::string& passphrase)
    : seed()
    , mnemonic()
    , passphrase(passphrase)
{
    char mnemonic_chars[HDWallet::maxMnemomincSize];
    mnemonic_generate(strength, mnemonic_chars);
    mnemonic_to_seed(mnemonic_chars, passphrase.c_str(), seed.data(), nullptr);
    mnemonic = mnemonic_chars;
}

HDWallet::HDWallet(const std::string& mnemonic, const std::string& passphrase)
    : seed()
    , mnemonic(mnemonic)
    , passphrase(passphrase)
{
    mnemonic_to_seed(mnemonic.c_str(), passphrase.c_str(), seed.data(), nullptr);
}

HDWallet::HDWallet(const Data& data, const std::string& passphrase)
    : seed()
    , mnemonic()
    , passphrase(passphrase)
{
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

PrivateKey HDWallet::getKey(const DerivationPath& derivationPath) const {
    const auto curve = TWCoinTypeCurve(derivationPath.coin());
    auto node = getNode(*this, curve, derivationPath);
    auto data = Data(node.private_key, node.private_key + PrivateKey::size);
    return PrivateKey(data);
}

std::string HDWallet::getExtendedPrivateKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const {
    const auto curve = TWCoinTypeCurve(coin);
    auto derivationPath = TW::DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(coin, true)});
    auto node = getNode(*this, curve, derivationPath);
    char buffer[HDWallet::maxExtendedKeySize] = {0};
    auto fingerprint = hdnode_fingerprint(&node);
    hdnode_private_ckd(&node, 0x80000000);
    hdnode_serialize_private(&node, fingerprint, version, buffer, HDWallet::maxExtendedKeySize);
    return buffer;
}

std::string HDWallet::getExtendedPublicKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const {
    const auto curve = TWCoinTypeCurve(coin);
    auto derivationPath = TW::DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(coin, true)});
    auto node = getNode(*this, curve, derivationPath);
    char buffer[HDWallet::maxExtendedKeySize] = {0};
    auto fingerprint = hdnode_fingerprint(&node);
    hdnode_private_ckd(&node, 0x80000000);
    hdnode_fill_public_key(&node);
    hdnode_serialize_public(&node, fingerprint, version, buffer, maxExtendedKeySize);
    return buffer;
}

PublicKey HDWallet::getPublicKeyFromExtended(const std::string& extended, TWCurve curve, enum TWHDVersion versionPublic, enum TWHDVersion versionPrivate, uint32_t change, uint32_t address) {
    auto node = HDNode{};
    uint32_t fingerprint = 0;

    hdnode_deserialize(extended.c_str(), versionPublic, versionPrivate, curveName(curve), &node, &fingerprint);
    hdnode_public_ckd(&node, change);
    hdnode_public_ckd(&node, address);
    hdnode_fill_public_key(&node);

    return PublicKey(Data(node.public_key, node.public_key + 33));
}

std::optional<std::string> HDWallet::getAddressFromExtended(const std::string& extended, TWCurve curve, TWCoinType coinType, uint32_t change, uint32_t address) {
	uint8_t data[78];
	if (base58_decode_check(extended.c_str(), HASHER_SHA2D, data, sizeof(data)) != sizeof(data)) {
		return nullptr;
	}

	TWHDVersion version = (TWHDVersion) read_be(data);
    if (version != TWHDVersionXPUB && version != TWHDVersionYPUB && version != TWHDVersionLTUB && version != TWHDVersionZPUB && version != TWHDVersionMTUB) {
        // Not a public key
        return {};
    }

    auto publicKey = HDWallet::getPublicKeyFromExtended(extended, curve, version, TWHDVersionNone, change, address);

    std::string string;
    switch (coinType) {
    case TWCoinTypeBitcoin: {
        auto address = Bitcoin::Bech32Address(reinterpret_cast<PublicKey&>(publicKey), 0, HRP_BITCOIN);
        string = address.string();
    } break;
    case TWCoinTypeLitecoin: {
        auto address = Bitcoin::Bech32Address(reinterpret_cast<PublicKey&>(publicKey), 0, HRP_LITECOIN);
        string = address.string();
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
    case TWCoinTypeRipple: {
        auto address = Ripple::Address(reinterpret_cast<PublicKey&>(publicKey));
        string = address.string();
    } break;
    default:
        // Unknown coin
        return nullptr;
    }

    return string;
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
        case TWCurveSECP256k1: return SECP256K1_NAME;
        case TWCurveEd25519: return ED25519_NAME;
        case TWCurveNIST256p1: return NIST256P1_NAME;
        default: return "";
        }
    }
}
