// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "MessageSigner.h"
#include "Address.h"

#include "Base64.h"
#include "BinaryCoding.h"
#include "Coin.h"
#include "Data.h"
#include "HexCoding.h"

using namespace TW;

namespace TW::Bitcoin {

Data msgToData(const std::string& msg) {
    Data d;
    TW::encodeVarInt(msg.size(), d);
    TW::append(d, TW::data(msg));
    return d;
}

Data msgToFullData(const std::string& msg) {
    Data d = msgToData(MessageSigner::MessagePrefix);
    TW::append(d, msgToData(msg));
    return d;
}

Data msgToHash(const std::string& msg) {
    Data d = msgToFullData(msg);
    return Hash::sha256d(d.data(), d.size());
}

std::string MessageSigner::signMessage(const PrivateKey& privateKey, const std::string& address, const std::string& message, bool compressed) {
    if (!Address::isValid(address)) {
        throw std::invalid_argument("Address is not valid (legacy) address");
    }
    std::string addrFromKey = "";
    if (compressed) {
        const auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        addrFromKey = Address(pubKey, TW::p2pkhPrefix(TWCoinTypeBitcoin)).string();
    } else {
        const auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        const auto keyHash = pubKey.hash(Data{TW::p2pkhPrefix(TWCoinTypeBitcoin)}, Hash::HasherSha256ripemd);
        addrFromKey = Address(keyHash).string();
    }
    if (addrFromKey != address) {
        throw std::invalid_argument("Address does not match key");
    }
    const auto messageHash = msgToHash(message);
    const auto signature = privateKey.sign(messageHash, TWCurveSECP256k1);

    // The V value: add 31 (or 27 for compressed), and move to the first byte
    const byte v = signature[64] + 27ul + (compressed ? 4ul : 0ul);
    auto sigAdjusted = Data{v};
    TW::append(sigAdjusted, TW::subData(signature, 0, 64));
    return Base64::encode(sigAdjusted);
}

/// May throw
std::string recoverAddressFromMessage(const std::string& message, const Data& signature) {
    if (signature.size() < 65) {
        throw std::invalid_argument("signature too short");
    }
    const auto messageHash = msgToHash(message);
    auto recId = signature[0];
    auto compressed = false;
    if (recId >= 31) {
        recId -= 4;
        compressed = true;
    }
    if (recId >= 27) {
        recId -= 27;
    }

    const auto publicKeyRecovered = PublicKey::recoverRaw(TW::subData(signature, 1), recId, messageHash);

    if (!compressed) {
        // uncompressed public key
        const auto keyHash = publicKeyRecovered.hash(Data{TW::p2pkhPrefix(TWCoinTypeBitcoin)}, Hash::HasherSha256ripemd);
        return Bitcoin::Address(keyHash).string();
    }
    // compressed
    const auto publicKeyRecoveredCompressed = publicKeyRecovered.compressed();
    return Bitcoin::Address(publicKeyRecoveredCompressed, TW::p2pkhPrefix(TWCoinTypeBitcoin)).string();
}

bool MessageSigner::verifyMessage(const std::string& address, const std::string& message, const std::string& signature) noexcept {
    try {
        const auto signatureData = Base64::decode(signature);
        return verifyMessage(address, message, signatureData);
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
