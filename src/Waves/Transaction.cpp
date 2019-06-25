// Copyright © 2017-2019 Trust Wallet.
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
using namespace TW::Waves;

using json = nlohmann::json;

const std::string Transaction::WAVES = "WAVES";

Data Transaction::serializeToSign() const {
    auto data = Data();
    if (attachment.size() > 140) {
        throw std::invalid_argument("Maximum attachment size is 140 bytes");
    }
    if (pub_key.empty()) {
        throw std::invalid_argument("Public key can't be empty");
    }
    data.resize(2);
    data[0] = static_cast<byte>(TransactionType::transfer);
    data[1] = static_cast<byte>(TransactionVersion::V2);
    append(data, pub_key);

    if (asset == WAVES) {
        data.push_back(static_cast<uint8_t>(0));
    } else {
        data.push_back(static_cast<uint8_t>(1));
        append(data, Base58::bitcoin.decode(asset));
    }

    if (fee_asset == WAVES) {
        data.push_back(static_cast<uint8_t>(0));
    } else {
        data.push_back(static_cast<uint8_t>(1));
        append(data, Base58::bitcoin.decode(fee_asset));
    }

    encode64BE(timestamp, data);
    encode64BE(amount, data);
    encode64BE(fee, data);
    append(data, Data(std::begin(to.bytes), std::end(to.bytes)));
    encodeDynamicLengthBytes(attachment, data);
    return data;
}

json Transaction::buildJson(Data signature) const {
    json jsonTx;

    jsonTx["type"] = TransactionType::transfer;
    jsonTx["version"] = TransactionVersion::V2;
    jsonTx["fee"] = fee;
    jsonTx["senderPublicKey"] = Base58::bitcoin.encode(pub_key);
    jsonTx["timestamp"] = timestamp;
    jsonTx["proofs"] = json::array({Base58::bitcoin.encode(signature)}).dump();
    jsonTx["recipient"] = Address(to).string();
    if (asset != WAVES) {
        jsonTx["assetId"] = asset;
    }
    if (fee_asset != WAVES) {
        jsonTx["feeAssetId"] = fee_asset;
    }
    jsonTx["amount"] = amount;
    jsonTx["attachment"] = Base58::bitcoin.encode(attachment);

    return jsonTx;
}