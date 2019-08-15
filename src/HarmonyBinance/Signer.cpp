// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"
#include "Signer.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <string>

using namespace TW;
using namespace TW::Binance;

// Message prefixes
static const auto sendOrderPrefix = std::vector<uint8_t>{0x2A, 0x2C, 0x87, 0xFA};
static const auto tradeOrderPrefix = std::vector<uint8_t>{0xCE, 0x6D, 0xC0, 0x43};
static const auto cancelTradeOrderPrefix = std::vector<uint8_t>{0x16, 0x6E, 0x68, 0x1B};
static const auto tokenFreezeOrderPrefix = std::vector<uint8_t>{0xE7, 0x74, 0xB3, 0x2D};
static const auto tokenUnfreezeOrderPrefix = std::vector<uint8_t>{0x65, 0x15, 0xFF, 0x0D};
static const auto pubKeyPrefix = std::vector<uint8_t>{0xEB, 0x5A, 0xE9, 0x87};
static const auto transactionPrefix = std::vector<uint8_t>{0xF0, 0x62, 0x5D, 0xEE};

std::vector<uint8_t> Signer::build() const {
    auto signature = encodeSignature(sign());
    return encodeTransaction(signature);
}

std::vector<uint8_t> Signer::sign() const {
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(signaturePreimage());
    auto signature = key.sign(hash, TWCurveSECP256k1);
    return std::vector<uint8_t>(signature.begin(), signature.end() - 1);
}

std::string Signer::signaturePreimage() const {
    auto json = signatureJSON(input);
    return json.dump();
}

std::vector<uint8_t> Signer::encodeTransaction(const std::vector<uint8_t>& signature) const {
    auto msg = encodeOrder();
    auto transaction = Binance::Proto::Transaction();
    transaction.add_msgs(msg.data(), msg.size());
    transaction.add_signatures(signature.data(), signature.size());
    transaction.set_memo(input.memo());
    transaction.set_source(input.source());

    auto data = transaction.SerializeAsString();
    return aminoWrap(data, transactionPrefix, true);
}

std::vector<uint8_t> Signer::encodeOrder() const {
    std::string data;
    std::vector<uint8_t> prefix;
    if (input.has_trade_order()) {
        data = input.trade_order().SerializeAsString();
        prefix = tradeOrderPrefix;
    } else if (input.has_cancel_trade_order()) {
        data = input.cancel_trade_order().SerializeAsString();
        prefix = cancelTradeOrderPrefix;
    } else if (input.has_send_order()) {
        data = input.send_order().SerializeAsString();
        prefix = sendOrderPrefix;
    } else if (input.has_freeze_order()) {
        data = input.freeze_order().SerializeAsString();
        prefix = tokenFreezeOrderPrefix;
    } else if (input.has_unfreeze_order()) {
        data = input.unfreeze_order().SerializeAsString();
        prefix = tokenUnfreezeOrderPrefix;
    } else {
        return {};
    }
    return aminoWrap(data, prefix, false);
}

std::vector<uint8_t> Signer::encodeSignature(const std::vector<uint8_t>& signature) const {
    auto key = PrivateKey(input.private_key());
    auto publicKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);

    auto encodedPublicKey = pubKeyPrefix;
    encodedPublicKey.insert(encodedPublicKey.end(), static_cast<uint8_t>(publicKey.bytes.size()));
    encodedPublicKey.insert(encodedPublicKey.end(), publicKey.bytes.begin(), publicKey.bytes.end());

    auto object = Binance::Proto::Signature();
    object.set_pub_key(encodedPublicKey.data(), encodedPublicKey.size());
    object.set_signature(signature.data(), signature.size());
    object.set_account_number(input.account_number());
    object.set_sequence(input.sequence());

    return aminoWrap(object.SerializeAsString(), {}, false);
}

std::vector<uint8_t> Signer::aminoWrap(const std::string& raw,
                                       const std::vector<uint8_t>& typePrefix,
                                       bool prefixWithSize) const {
    const auto contentsSize = raw.size() + typePrefix.size();
    auto size = contentsSize;
    if (prefixWithSize) {
        size += 10;
    }

    std::string msg;
    msg.reserve(size);
    {
        google::protobuf::io::StringOutputStream output(&msg);
        google::protobuf::io::CodedOutputStream cos(&output);
        if (prefixWithSize) {
            cos.WriteVarint64(contentsSize);
        }
        cos.WriteRaw(typePrefix.data(), static_cast<int>(typePrefix.size()));
        cos.WriteRaw(raw.data(), static_cast<int>(raw.size()));
    }

    return std::vector<uint8_t>(msg.begin(), msg.end());
}
