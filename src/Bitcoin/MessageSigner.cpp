// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "MessageSigner.h"
#include "Address.h"

#include "Base64.h"
#include "BinaryCoding.h"
#include "Coin.h"
#include "Data.h"
#include "HexCoding.h"

using namespace TW;

namespace TW::Bitcoin {

// length-encode a message string
Data messageToData(const std::string& message) {
    Data d;
    TW::encodeVarInt(message.size(), d);
    TW::append(d, TW::data(message));
    return d;
}

// append prefix and length-encode message string
Data messageToFullData(const std::string& message) {
    Data d = messageToData(MessageSigner::MessagePrefix);
    TW::append(d, messageToData(message));
    return d;
}

Data MessageSigner::messageToHash(const std::string& message) {
    Data d = messageToFullData(message);
    return Hash::sha256d(d.data(), d.size());
}

// Returns why `address` cannot be signed for with `privateKey`, or nullptr if it can.
static const char* signingAddressError(const PrivateKey& privateKey, const std::string& address, bool compressed) {
    if (!Address::isValid(address)) {
        return "Address is not valid (legacy) address";
    }
    if (!PrivateKey::isValid(privateKey.key(), TWCurveSECP256k1)) {
        return "Invalid private key";
    }
    std::string addrFromKey;
    if (compressed) {
        const auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        addrFromKey = Address(pubKey, TW::p2pkhPrefix(TWCoinTypeBitcoin)).string();
    } else {
        const auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        const auto keyHash = pubKey.hash(Data{TW::p2pkhPrefix(TWCoinTypeBitcoin)}, Hash::HasherSha256ripemd);
        addrFromKey = Address(keyHash).string();
    }
    if (addrFromKey != address) {
        return "Address does not match key";
    }
    return nullptr;
}

static std::string signValidated(const PrivateKey& privateKey, const std::string& message, bool compressed) {
    const auto messageHash = MessageSigner::messageToHash(message);
    const auto signature = privateKey.sign(messageHash, TWCurveSECP256k1);

    // The V value: add 31 (or 27 for compressed), and move to the first byte
    const byte v = signature[MessageSigner::SignatureRSLength] + PublicKey::SignatureVOffset + (compressed ? 4ul : 0ul);
    auto sigAdjusted = Data{v};
    TW::append(sigAdjusted, TW::subData(signature, 0, MessageSigner::SignatureRSLength));
    return Base64::encode(sigAdjusted);
}

std::string MessageSigner::signMessage(const PrivateKey& privateKey, const std::string& address, const std::string& message, bool compressed) {
    if (const auto* error = signingAddressError(privateKey, address, compressed)) {
        throw std::invalid_argument(error);
    }
    return signValidated(privateKey, message, compressed);
}

std::optional<std::string> MessageSigner::trySignMessage(const PrivateKey& privateKey, const std::string& address, const std::string& message, bool compressed) {
    if (signingAddressError(privateKey, address, compressed) != nullptr) {
        return std::nullopt;
    }
    return signValidated(privateKey, message, compressed);
}

std::string MessageSigner::recoverAddressFromMessage(const std::string& message, const Data& signature) {
    // Checked here as well to keep the specific message.
    if (signature.size() < SignatureRSVLength) {
        throw std::invalid_argument("signature too short");
    }
    if (auto address = tryRecoverAddressFromMessage(message, signature)) {
        return *address;
    }
    throw std::invalid_argument("recover failed");
}

std::optional<std::string> MessageSigner::tryRecoverAddressFromMessage(const std::string& message, const Data& signature) {
    if (signature.size() < SignatureRSVLength) {
        return std::nullopt;
    }
    const auto messageHash = MessageSigner::messageToHash(message);
    auto recId = signature[0];
    auto compressed = false;
    if (recId >= PublicKey::SignatureVOffset + 4) {
        recId -= 4;
        compressed = true;
    }
    if (recId >= PublicKey::SignatureVOffset) {
        recId -= PublicKey::SignatureVOffset;
    }

    const auto publicKeyRecovered = PublicKey::tryRecoverRaw(TW::subData(signature, 1), recId, messageHash);
    if (!publicKeyRecovered) {
        return std::nullopt;
    }

    if (!compressed) {
        // uncompressed public key
        const auto keyHash = publicKeyRecovered->hash(Data{TW::p2pkhPrefix(TWCoinTypeBitcoin)}, Hash::HasherSha256ripemd);
        return Bitcoin::Address(keyHash).string();
    }
    // compressed
    const auto publicKeyRecoveredCompressed = publicKeyRecovered->compressed();
    return Bitcoin::Address(publicKeyRecoveredCompressed, TW::p2pkhPrefix(TWCoinTypeBitcoin)).string();
}

bool MessageSigner::verifyMessage(const std::string& address, const std::string& message, const std::string& signature) noexcept {
    try {
        if (!Bitcoin::Address::isValid(address)) {
            return false;
        }
        const auto addressRecovered = tryRecoverAddressFromMessage(message, Base64::decode(signature));
        return addressRecovered.has_value() && *addressRecovered == address;
    } catch (...) {
        return false;
    }
}

/// May throw
bool MessageSigner::verifyMessage(const std::string& address, const std::string& message, const Data& signature) {
    if (!Bitcoin::Address::isValid(address)) {
        throw std::invalid_argument("Input address invalid, must be valid legacy");
    }
    const auto addressRecovered = recoverAddressFromMessage(message, signature);
    return (addressRecovered == address);
}

} // namespace TW::Bitcoin
