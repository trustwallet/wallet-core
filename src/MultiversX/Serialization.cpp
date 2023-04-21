// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "Address.h"
#include "Base64.h"

using namespace TW;

std::map<std::string, int> fields_order{
    {"nonce", 1},
    {"value", 2},
    {"receiver", 3},
    {"sender", 4},
    {"senderUsername", 5},
    {"receiverUsername", 6},
    {"gasPrice", 7},
    {"gasLimit", 8},
    {"data", 9},
    {"chainID", 10},
    {"version", 11},
    {"signature", 12},
    {"options", 13},
    {"guardian", 14}};

struct FieldsSorter {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
        return fields_order[lhs] < fields_order[rhs];
    }
};

template <class Key, class T, class Compare, class Allocator>
using sorted_map = std::map<Key, T, FieldsSorter, Allocator>;
using sorted_json = nlohmann::basic_json<sorted_map>;

sorted_json preparePayload(const MultiversX::Transaction& transaction) {
    using namespace nlohmann;
    sorted_json payload{
        {"nonce", json(transaction.nonce)},
        {"value", json(transaction.value)},
        {"receiver", json(transaction.receiver)},
        {"sender", json(transaction.sender)},
        {"gasPrice", json(transaction.gasPrice)},
        {"gasLimit", json(transaction.gasLimit)},
    };

    if (!transaction.senderUsername.empty()) {
        payload["senderUsername"] = json(Base64::encode(data(transaction.senderUsername)));
    }

    if (!transaction.receiverUsername.empty()) {
        payload["receiverUsername"] = json(Base64::encode(data(transaction.receiverUsername)));
    }

    if (!transaction.data.empty()) {
        payload["data"] = json(Base64::encode(data(transaction.data)));
    }

    payload["chainID"] = json(transaction.chainID);
    payload["version"] = json(transaction.version);

    if (transaction.options != 0) {
        payload["options"] = json(transaction.options);
    }

    if (!transaction.guardian.empty()) {
        payload["guardian"] = json(transaction.guardian);
    }

    return payload;
}

std::string MultiversX::serializeTransaction(const MultiversX::Transaction& transaction) {
    sorted_json payload = preparePayload(transaction);
    return payload.dump();
}

std::string MultiversX::serializeSignedTransaction(const MultiversX::Transaction& transaction, std::string signature) {
    sorted_json payload = preparePayload(transaction);
    payload["signature"] = nlohmann::json(signature);
    return payload.dump();
}
