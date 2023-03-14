// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "BinaryCoding.h"
#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

using namespace TW;

namespace TW::Waves {

using json = nlohmann::json;

const std::string Transaction::WAVES = "WAVES";

Data serializeTransfer(int64_t amount, std::string asset, int64_t fee, std::string fee_asset, Address to, const Data& attachment, int64_t timestamp, const Data& pub_key) {
    auto data = Data();
    if (asset.empty()) {
        asset = Transaction::WAVES;
    }
    if (fee_asset.empty()) {
        fee_asset = Transaction::WAVES;
    }
    data.resize(2);
    data[0] = static_cast<byte>(TransactionType::transfer);
    data[1] = static_cast<byte>(TransactionVersion::V2);
    append(data, pub_key);
    if (asset == Transaction::WAVES) {
        data.push_back(static_cast<uint8_t>(0));
    } else {
        data.push_back(static_cast<uint8_t>(1));
        append(data, Base58::decode(asset));
    }
    if (fee_asset == Transaction::WAVES) {
        data.push_back(static_cast<uint8_t>(0));
    } else {
        data.push_back(static_cast<uint8_t>(1));
        append(data, Base58::decode(fee_asset));
    }
    encode64BE(timestamp, data);
    encode64BE(amount, data);
    encode64BE(fee, data);
    append(data, Data(std::begin(to.bytes), std::end(to.bytes)));
    encodeDynamicLengthBytes(attachment, data);

    return data;
}

Data serializeLease(int64_t amount, int64_t fee, Address to, int64_t timestamp, const Data& pub_key) {
    auto data = Data();
    data.resize(2);
    data[0] = static_cast<byte>(TransactionType::lease);
    data[1] = static_cast<byte>(TransactionVersion::V2);
    data.push_back(static_cast<uint8_t>(0));
    append(data, pub_key);
    append(data, Data(std::begin(to.bytes), std::end(to.bytes)));
    encode64BE(amount, data);
    encode64BE(fee, data);
    encode64BE(timestamp, data);

    return data;
}

Data serializeCancelLease(const Data& leaseId, int64_t fee, int64_t timestamp, const Data& pub_key) {
    auto data = Data();
    data.resize(2);
    data[0] = static_cast<byte>(TransactionType::cancelLease);
    data[1] = static_cast<byte>(TransactionVersion::V2);
    data.push_back(static_cast<uint8_t>(87));
    append(data, pub_key);
    encode64BE(fee, data);
    encode64BE(timestamp, data);
    append(data, leaseId);

    return data;
}

json jsonTransfer(const Data& signature, int64_t amount, const std::string& asset, int64_t fee, const std::string& fee_asset, Address to, const Data& attachment, int64_t timestamp, const Data& pub_key) {
    json jsonTx;

    jsonTx["type"] = TransactionType::transfer;
    jsonTx["version"] = TransactionVersion::V2;
    jsonTx["fee"] = fee;
    jsonTx["senderPublicKey"] = Base58::encode(pub_key);
    jsonTx["timestamp"] = timestamp;
    jsonTx["proofs"] = json::array({Base58::encode(signature)});
    jsonTx["recipient"] = Address(to).string();
    if (asset != Transaction::WAVES) {
        jsonTx["assetId"] = asset;
    }
    if (fee_asset != Transaction::WAVES) {
        jsonTx["feeAssetId"] = fee_asset;
    }
    jsonTx["amount"] = amount;
    jsonTx["attachment"] = Base58::encode(attachment);

    return jsonTx;
}

json jsonLease(const Data& signature, int64_t amount, int64_t fee, Address to, int64_t timestamp, const Data& pub_key) {
    json jsonTx;

    jsonTx["type"] = TransactionType::lease;
    jsonTx["version"] = TransactionVersion::V2;
    jsonTx["fee"] = fee;
    jsonTx["senderPublicKey"] = Base58::encode(pub_key);
    jsonTx["timestamp"] = timestamp;
    jsonTx["proofs"] = json::array({Base58::encode(signature)});
    jsonTx["recipient"] = Address(to).string();
    jsonTx["amount"] = amount;

    return jsonTx;
}

json jsonCancelLease(const Data& signature, const Data& leaseId, int64_t fee, int64_t timestamp, const Data& pub_key) {
    json jsonTx;

    jsonTx["type"] = TransactionType::cancelLease;
    jsonTx["version"] = TransactionVersion::V2;
    jsonTx["fee"] = fee;
    jsonTx["senderPublicKey"] = Base58::encode(pub_key);
    jsonTx["leaseId"] = Base58::encode(leaseId);
    jsonTx["chainId"] = 87; // mainnet
    jsonTx["timestamp"] = timestamp;
    jsonTx["proofs"] = json::array({Base58::encode(signature)});

    return jsonTx;
}

Data Transaction::serializeToSign() const {
    if (pub_key.empty()) {
        throw std::invalid_argument("Public key can't be empty");
    }
    if (input.has_transfer_message()) {
        auto message = input.transfer_message();
        auto attachment =
            Data(message.attachment().begin(), message.attachment().end());
        if (attachment.size() > 140) {
            throw std::invalid_argument("Maximum attachment size is 140 bytes");
        }
        return serializeTransfer(message.amount(), message.asset(),
                                 message.fee(), message.fee_asset(),
                                 Address(message.to()), attachment,
                                 input.timestamp(), pub_key);
    } else if (input.has_lease_message()) {
        auto message = input.lease_message();
        return serializeLease(message.amount(), message.fee(), Address(message.to()), input.timestamp(), pub_key);
    } else if (input.has_cancel_lease_message()) {
        auto message = input.cancel_lease_message();
        auto leaseId = Base58::decode(message.lease_id());
        return serializeCancelLease(leaseId, message.fee(), input.timestamp(), pub_key);
    }

    return Data();
}

json Transaction::buildJson(const Data& signature) const {
    if (input.has_transfer_message()) {
        auto message = input.transfer_message();
        auto attachment = Data(message.attachment().begin(), message.attachment().end());
        return jsonTransfer(
            signature,
            message.amount(),
            message.asset(),
            message.fee(),
            message.fee_asset(),
            Address(message.to()),
            attachment,
            input.timestamp(),
            pub_key);
    } else if (input.has_lease_message()) {
        auto message = input.lease_message();
        return jsonLease(
            signature,
            message.amount(),
            message.fee(),
            Address(message.to()),
            input.timestamp(),
            pub_key);
    } else if (input.has_cancel_lease_message()) {
        auto message = input.cancel_lease_message();
        auto leaseId = Base58::decode(message.lease_id());
        return jsonCancelLease(
            signature,
            leaseId,
            message.fee(),
            input.timestamp(),
            pub_key);
    }
    return nullptr;
}

} // namespace TW::Waves
