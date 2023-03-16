// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Protobuf/ZilliqaMessage.pb.h"

#include "Data.h"
#include "Hash.h"
#include "HexCoding.h"
#include "uint256.h"

#include <cassert>

#include <google/protobuf/util/json_util.h>
#include <nlohmann/json.hpp>

namespace TW::Zilliqa {

using ByteArray = ZilliqaMessage::ByteArray;

static inline Data prependZero(Data& data) {
    if (data.size() < 16) {
        Data zero = Data(16 - data.size(), 0);
        zero.insert(zero.end(), data.begin(), data.end());
        return zero;
    }
    return Data(data);
}

static inline ByteArray* byteArray(Data& amount) {
    auto* array = new ByteArray();
    amount = prependZero(amount);
    array->set_data(amount.data(), amount.size());
    return array;
}

static inline ByteArray* byteArray(const void* data, size_t size) {
    auto* array = new ByteArray();
    array->set_data(data, size);
    return array;
}

Data Signer::getPreImage(const Proto::SigningInput& input, Address& address) noexcept {
    auto internal = ZilliqaMessage::ProtoTransactionCoreInfo();
    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    if (!Address::decode(input.to(), address)) {
        // invalid input address
        return Data(0);
    }
    const auto pubKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto gasPrice = Data(input.gas_price().begin(), input.gas_price().end());

    internal.set_version(input.version());
    internal.set_nonce(input.nonce());
    internal.set_toaddr(address.getKeyHash().data(), address.getKeyHash().size());
    internal.set_gaslimit(input.gas_limit());
    internal.set_allocated_senderpubkey(byteArray(pubKey.bytes.data(), pubKey.bytes.size()));
    internal.set_allocated_gasprice(byteArray(gasPrice));

    Data amount;
    switch (input.transaction().message_oneof_case()) {
    case Proto::Transaction::kTransfer: {
        const auto transfer = input.transaction().transfer();
        amount = Data(transfer.amount().begin(), transfer.amount().end());
        break;
    }
    case Proto::Transaction::kRawTransaction: {
        const auto raw = input.transaction().raw_transaction();
        amount = Data(raw.amount().begin(), raw.amount().end());
        if (!raw.code().empty()) {
            internal.set_code(raw.code());
        }
        if (!raw.data().empty()) {
            internal.set_data(raw.data());
        }
        break;
    }
    default:
        break;
    }

    internal.set_allocated_amount(byteArray(amount));

    const auto serialized = internal.SerializeAsString();
    return Data(serialized.begin(), serialized.end());
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();
    Address address;
    const auto preImage = Signer::getPreImage(input, address);
    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    const auto pubKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto signature = key.signZilliqa(preImage);
    const auto transaction = input.transaction();

    // build json
    nlohmann::json json = {
        {"version", input.version()},
        {"toAddr", address.checksumed()},
        {"nonce", input.nonce()},
        {"gasPrice", toString(load(input.gas_price()))},
        {"gasLimit", toString(input.gas_limit())},
        {"code", ""},
        {"data", ""},
        {"pubKey", hex(pubKey.bytes)},
        {"signature", hex(signature)},
    };

    if (transaction.has_transfer()) {
        const auto transfer = transaction.transfer();
        json["amount"] = toString(load(transfer.amount()));
    } else if (transaction.has_raw_transaction()) {
        const auto raw = transaction.raw_transaction();
        json["amount"] = toString(load(raw.amount()));
        if (!raw.code().empty()) {
            json["code"] = hex(Data(raw.code().begin(), raw.code().end()));
        }
        if (!raw.data().empty()) {
            json["data"] = std::string(raw.data().begin(), raw.data().end());
        }
    }

    output.set_json(json.dump());
    output.set_signature(signature.data(), signature.size());

    return output;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    return hex(Signer::sign(input).json());
}

} // namespace TW::Zilliqa
