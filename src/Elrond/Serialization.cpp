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
    {"gasPrice", 5},
    {"gasLimit", 6},
    {"data", 7},
    {"signature", 8}
};

struct FieldsSorter {
    bool operator() (const string& lhs, const string& rhs) const {
        return fields_order[lhs] < fields_order[rhs]; 
    }
};

template<class Key, class T, class Compare, class Allocator>
using sorted_map = std::map<Key, T, FieldsSorter, Allocator>;
using sorted_json = nlohmann::basic_json<sorted_map>;

string Elrond::serializeTransaction(const Proto::TransactionMessage& message) {
    sorted_json payload {
        {"nonce", json(message.nonce())},
        {"value", json(message.value())},
        {"receiver", json(message.receiver())},
        {"sender", json(message.sender())},
        {"gasPrice", json(message.gas_price())},
        {"gasLimit", json(message.gas_limit())},
    };

    if (!message.data().empty()) {
        payload["data"] = json(TW::Base64::encode(TW::data(message.data())));
    }

    return payload.dump();
}

string Elrond::serializeSignedTransaction(const Proto::TransactionMessage& message, string signature) {
    sorted_json payload {
        {"nonce", json(message.nonce())},
        {"value", json(message.value())},
        {"receiver", json(message.receiver())},
        {"sender", json(message.sender())},
        {"gasPrice", json(message.gas_price())},
        {"gasLimit", json(message.gas_limit())},
        {"data", json(message.data())},
        {"signature", json(signature)},
    };

    return payload.dump();
}
