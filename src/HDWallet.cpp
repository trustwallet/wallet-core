// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HDWallet.h"

#include "Base58.h"
#include "BinaryCoding.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/CashAddress.h"
#include "Coin.h"
#include "Mnemonic.h"

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPublicKeyType.h>

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/bip39.h>
#include <TrezorCrypto/curves.h>
#include <TrezorCrypto/memzero.h>

#include <array>
#include <cstring>

using namespace TW;

namespace {

uint32_t fingerprint(HDNode *node, Hash::Hasher hasher);
std::string serialize(const HDNode *node, uint32_t fingerprint, uint32_t version, bool use_public, Hash::Hasher hasher);
bool deserialize(const std::string& extended, TWCurve curve, Hash::Hasher hasher, HDNode *node);
HDNode getNode(const HDWallet& wallet, TWCurve curve, const DerivationPath& derivationPath);
HDNode getMasterNode(const HDWallet& wallet, TWCurve curve);

const char* curveName(TWCurve curve);
} // namespace

const int MnemonicBufLength = Mnemonic::MaxWords * (BIP39_MAX_WORD_LENGTH + 3) + 20; // some extra slack

HDWallet::HDWallet(int strength, const std::string& passphrase)
    : passphrase(passphrase) {
    char buf[MnemonicBufLength];
    const char* mnemonic_chars = mnemonic_generate(strength, buf, MnemonicBufLength);
    if (mnemonic_chars == nullptr) {
        throw std::invalid_argument("Invalid strength");
    }
    mnemonic = mnemonic_chars;
    memzero(buf, MnemonicBufLength);
    updateSeedAndEntropy();
}

HDWallet::HDWallet(const std::string& mnemonic, const std::string& passphrase, const bool check)
    : mnemonic(mnemonic), passphrase(passphrase) {
    if (mnemonic.length() == 0 ||
        (check && !Mnemonic::isValid(mnemonic))) {
        throw std::invalid_argument("Invalid mnemonic");
    }
    updateSeedAndEntropy(check);
}

HDWallet::HDWallet(const Data& entropy, const std::string& passphrase)
    : passphrase(passphrase) {
    char buf[MnemonicBufLength];
    const char* mnemonic_chars = mnemonic_from_data(entropy.data(), static_cast<int>(entropy.size()), buf, MnemonicBufLength);
    if (mnemonic_chars == nullptr) {
        throw std::invalid_argument("Invalid mnemonic data");
    }
    mnemonic = mnemonic_chars;
    memzero(buf, MnemonicBufLength);
    updateSeedAndEntropy();
}

HDWallet::~HDWallet() {
    std::fill(seed.begin(), seed.end(), 0);
    std::fill(mnemonic.begin(), mnemonic.end(), 0);
    std::fill(passphrase.begin(), passphrase.end(), 0);
}

void HDWallet::updateSeedAndEntropy(bool check) {
    assert(!check || Mnemonic::isValid(mnemonic)); // precondition

    // generate seed from mnemonic
    mnemonic_to_seed(mnemonic.c_str(), passphrase.c_str(), seed.data(), nullptr);

    // generate entropy bits from mnemonic
    Data entropyRaw((Mnemonic::MaxWords * Mnemonic::BitsPerWord) / 8);
    // entropy is truncated to fully bytes, 4 bytes for each 3 words (=33 bits)
    auto entropyBytes = mnemonic_to_bits(mnemonic.c_str(), entropyRaw.data()) / 33 * 4;
    // copy to truncate
    entropy = data(entropyRaw.data(), entropyBytes);
    assert(!check || entropy.size() > 10);
}

PrivateKey HDWallet::getMasterKey(TWCurve curve) const {
    auto node = getMasterNode(*this, curve);
    auto data = Data(node.private_key, node.private_key + PrivateKey::size);
    return PrivateKey(data);
}

PrivateKey HDWallet::getMasterKeyExtension(TWCurve curve) const {
    auto node = getMasterNode(*this, curve);
    auto data = Data(node.private_key_extension, node.private_key_extension + PrivateKey::size);
    return PrivateKey(data);
}

PrivateKey HDWallet::getKey(TWCoinType coin, TWDerivation derivation) const {
    const auto path = TW::derivationPath(coin, derivation);
    return getKey(coin, path);
}

DerivationPath HDWallet::cardanoStakingDerivationPath(const DerivationPath& path) {
    DerivationPath stakingPath = path;
    stakingPath.indices[3].value = 2;
    stakingPath.indices[4].value = 0;
    return stakingPath;
}

PrivateKey HDWallet::getKey(TWCoinType coin, const DerivationPath& derivationPath) const {
    const auto curve = TWCoinTypeCurve(coin);
    const auto privateKeyType = getPrivateKeyType(curve);
    const auto node = getNode(*this, curve, derivationPath);
    switch (privateKeyType) {
        case PrivateKeyTypeDoubleExtended: // special handling for Cardano
            {
                if (derivationPath.indices.size() < 4 || derivationPath.indices[3].value > 1) {
                    // invalid derivation path
                    return PrivateKey(Data(192));
                }
                const DerivationPath stakingPath = cardanoStakingDerivationPath(derivationPath);

                auto pkData = Data(node.private_key, node.private_key + PrivateKey::size);
                auto extData = Data(node.private_key_extension, node.private_key_extension + PrivateKey::size);
                auto chainCode = Data(node.chain_code, node.chain_code + PrivateKey::size);

                // repeat with staking path
                const auto node2 = getNode(*this, curve, stakingPath);
                auto pkData2 = Data(node2.private_key, node2.private_key + PrivateKey::size);
                auto extData2 = Data(node2.private_key_extension, node2.private_key_extension + PrivateKey::size);
                auto chainCode2 = Data(node2.chain_code, node2.chain_code + PrivateKey::size);

                return PrivateKey(pkData, extData, chainCode, pkData2, extData2, chainCode2);
            }

        case PrivateKeyTypeDefault32:
        default:
            // default path
            auto data = Data(node.private_key, node.private_key + PrivateKey::size);
            return PrivateKey(data);
    }
}

std::string HDWallet::getRootKey(TWCoinType coin, TWHDVersion version) const {
    const auto curve = TWCoinTypeCurve(coin);
    auto node = getMasterNode(*this, curve);
    return serialize(&node, 0, version, false, base58Hasher(coin));
}

std::string HDWallet::deriveAddress(TWCoinType coin) const {
    return deriveAddress(coin, TWDerivationDefault);
}

std::string HDWallet::deriveAddress(TWCoinType coin, TWDerivation derivation) const {
    const auto derivationPath = TW::derivationPath(coin, derivation);
    return TW::deriveAddress(coin, getKey(coin, derivationPath), derivation);
}

std::string HDWallet::getExtendedPrivateKeyAccount(TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version, uint32_t account) const {
    if (version == TWHDVersionNone) {
        return "";
    }
    
    const auto curve = TWCoinTypeCurve(coin);
    const auto path = TW::derivationPath(coin, derivation);
    auto derivationPath = DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(path.coin(), true)});
    auto node = getNode(*this, curve, derivationPath);
    auto fingerprintValue = fingerprint(&node, publicKeyHasher(coin));
    hdnode_private_ckd(&node, account + 0x80000000);
    return serialize(&node, fingerprintValue, version, false, base58Hasher(coin));
}

std::string HDWallet::getExtendedPublicKeyAccount(TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version, uint32_t account) const {
    if (version == TWHDVersionNone) {
        return "";
    }
    
    const auto curve = TWCoinTypeCurve(coin);
    const auto path = TW::derivationPath(coin, derivation);
    auto derivationPath = DerivationPath({DerivationPathIndex(purpose, true), DerivationPathIndex(path.coin(), true)});
    auto node = getNode(*this, curve, derivationPath);
    auto fingerprintValue = fingerprint(&node, publicKeyHasher(coin));
    hdnode_private_ckd(&node, account + 0x80000000);
    hdnode_fill_public_key(&node);
    return serialize(&node, fingerprintValue, version, true, base58Hasher(coin));
}

std::optional<PublicKey> HDWallet::getPublicKeyFromExtended(const std::string& extended, TWCoinType coin, const DerivationPath& path) {
    const auto curve = TW::curve(coin);
    const auto hasher = TW::base58Hasher(coin);

    auto node = HDNode{};
    if (!deserialize(extended, curve, hasher, &node)) {
        return {};
    }
    if (node.curve->params == nullptr) {
        return {};
    }
    hdnode_public_ckd(&node, path.change());
    hdnode_public_ckd(&node, path.address());
    hdnode_fill_public_key(&node);

    // These public key type are not applicable.  Handled above, as node.curve->params is null
    assert(curve != TWCurveED25519 && curve != TWCurveED25519Blake2bNano && curve != TWCurveED25519Extended && curve != TWCurveCurve25519);
    TWPublicKeyType keyType = TW::publicKeyType(coin);
    if (curve == TWCurveSECP256k1) {
        auto pubkey = PublicKey(Data(node.public_key, node.public_key + 33), TWPublicKeyTypeSECP256k1);
        if (keyType == TWPublicKeyTypeSECP256k1Extended) {
            return pubkey.extended();
        } else {
            return pubkey;
        }
    } else if (curve == TWCurveNIST256p1) {
        auto pubkey = PublicKey(Data(node.public_key, node.public_key + 33), TWPublicKeyTypeNIST256p1);
        if (keyType == TWPublicKeyTypeNIST256p1Extended) {
            return pubkey.extended();
        } else {
            return pubkey;
        }
    }
    return {};
}

std::optional<PrivateKey> HDWallet::getPrivateKeyFromExtended(const std::string& extended, TWCoinType coin, const DerivationPath& path) {
    const auto curve = TW::curve(coin);
    const auto hasher = TW::base58Hasher(coin);

    auto node = HDNode{};
    if (!deserialize(extended, curve, hasher, &node)) {
        return {};
    }
    hdnode_private_ckd(&node, path.change());
    hdnode_private_ckd(&node, path.address());

    return PrivateKey(Data(node.private_key, node.private_key + 32));
}

HDWallet::PrivateKeyType HDWallet::getPrivateKeyType(TWCurve curve) {
    switch (curve) {
    case TWCurve::TWCurveED25519Extended:
        // used by Cardano
        return PrivateKeyTypeDoubleExtended;
    default:
        // default
        return PrivateKeyTypeDefault32;
    }
}

namespace {

uint32_t fingerprint(HDNode *node, Hash::Hasher hasher) {
    hdnode_fill_public_key(node);
    auto digest = Hash::hash(hasher, node->public_key, 33);
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

bool deserialize(const std::string& extended, TWCurve curve, Hash::Hasher hasher, HDNode* node) {
    memset(node, 0, sizeof(HDNode));
    const char* curveNameStr = curveName(curve);
    if (curveNameStr == nullptr || ::strlen(curveNameStr) == 0) {
        return false;
    }
    node->curve = get_curve_by_name(curveNameStr);
    assert(node->curve != nullptr);

    const auto node_data = Base58::bitcoin.decodeCheck(extended, hasher);
    if (node_data.size() != 78) {
        return false;
    }

    uint32_t version = decode32BE(node_data.data());
    if (TWHDVersionIsPublic(static_cast<TWHDVersion>(version))) {
        std::copy(node_data.begin() + 45, node_data.begin() + 45 + 33, node->public_key);
    } else if (TWHDVersionIsPrivate(static_cast<TWHDVersion>(version))) {
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
    const auto privateKeyType = HDWallet::getPrivateKeyType(curve);
    auto node = getMasterNode(wallet, curve);
    for (auto& index : derivationPath.indices) {
        switch (privateKeyType) {
            case HDWallet::PrivateKeyTypeDoubleExtended: // used by Cardano, special handling
                hdnode_private_ckd_cardano(&node, index.derivationIndex());
                break;
           case HDWallet::PrivateKeyTypeDefault32:
            default:
                hdnode_private_ckd(&node, index.derivationIndex());
                break;
        }
    }
    return node;
}

HDNode getMasterNode(const HDWallet& wallet, TWCurve curve) {
    const auto privateKeyType = HDWallet::getPrivateKeyType(curve);
    auto node = HDNode();
    switch (privateKeyType) {
        case HDWallet::PrivateKeyTypeDoubleExtended: // used by Cardano
            // special handling for extended, use entropy (not seed)
            hdnode_from_entropy_cardano_icarus((const uint8_t*)"", 0, wallet.getEntropy().data(), (int)wallet.getEntropy().size(), &node);
            break;

        case HDWallet::PrivateKeyTypeDefault32:
        default:
            hdnode_from_seed(wallet.getSeed().data(), HDWallet::seedSize, curveName(curve), &node);
            break;
    }
    return node;
}

const char* curveName(TWCurve curve) {
    switch (curve) {
    case TWCurveSECP256k1:
        return SECP256K1_NAME;
    case TWCurveED25519:
        return ED25519_NAME;
    case TWCurveED25519Blake2bNano:
        return ED25519_BLAKE2B_NANO_NAME;
    case TWCurveED25519Extended:
        return ED25519_CARDANO_NAME;
    case TWCurveNIST256p1:
        return NIST256P1_NAME;
    case TWCurveCurve25519:
        return CURVE25519_NAME;
    case TWCurveNone:
    default:
        return "";
    }
}

} // namespace
