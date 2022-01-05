// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../Elrond/Address.h"
#include "../proto/Elrond.pb.h"
#include "Base64.h"
#include "PrivateKey.h"

using namespace TW;

std::map<string, int> fields_order {
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
    {"options", 12},
    {"signature", 13}
};

struct FieldsSorter {
    bool operator() (const string& lhs, const string& rhs) const {
        return fields_order[lhs] < fields_order[rhs]; 
    }
};

template<class Key, class T, class Compare, class Allocator>
using sorted_map = std::map<Key, T, FieldsSorter, Allocator>;
using sorted_json = nlohmann::basic_json<sorted_map>;

sorted_json preparePayload(const Elrond::Proto::TransactionMessage& message) {
    sorted_json payload {
        {"nonce", json(message.nonce())},
        {"value", json(message.value())},
        {"receiver", json(message.receiver())},
        {"sender", json(message.sender())},
        {"gasPrice", json(message.gas_price())},
        {"gasLimit", json(message.gas_limit())},
    };

    if (!message.sender_username().empty()) {
        payload["senderUsername"] = json(Base64::encode(data(message.sender_username())));
    }

    if (!message.receiver_username().empty()) {
        payload["receiverUsername"] = json(Base64::encode(data(message.receiver_username())));
    }

    if (!message.data().empty()) {
        payload["data"] = json(Base64::encode(data(message.data())));
    }

    payload["chainID"] = json(message.chain_id());
    payload["version"] = json(message.version());

    if (message.options() != 0) {
        payload["options"] = json(message.options());
    }

    return payload;
}

string Elrond::serializeTransaction(const Proto::TransactionMessage& message) {
    sorted_json payload = preparePayload(message);
    return payload.dump();
}

string Elrond::serializeSignedTransaction(const Proto::TransactionMessage& message, string signature) {
    sorted_json payload = preparePayload(message);
    payload["signature"] = json(signature);
    return payload.dump();
}
