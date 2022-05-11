// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "AddressV3.h"

#include "Data.h"
#include "uint256.h"
#include "../proto/Cardano.pb.h"
#include "../proto/Common.pb.h"

#include <vector>
#include <string>

namespace TW::Cardano {

typedef uint64_t Amount;

class TokenAmount {
public:
    std::string policyId;
    std::string assetName;
    uint256_t amount;

    static TokenAmount fromProto(const Proto::TokenAmount& proto);
    Proto::TokenAmount toProto() const;
    std::string key() const { return policyId + "_" + assetName; }
};

class TokenBundle {
public:
    std::map<std::string, TokenAmount> bundle;
    void add(const TokenAmount& ta) {
        const auto key = ta.key();
        if (bundle.find(key) == bundle.end()) {
            bundle[key] = ta;
        } else {
            auto entry = bundle[key];
            entry.amount += ta.amount;
            bundle[key] = entry;
        }
    }
    uint256_t amount(const std::string& key) const {
        const auto& findkey = bundle.find(key);
        if (findkey == bundle.end()) {
            return 0;
        }
        return findkey->second.amount;
    }
    size_t size() const { return bundle.size(); }
};

class OutPoint {
public:
    Data txHash;
    uint64_t outputIndex;

    static OutPoint fromProto(const Proto::OutPoint& proto);
};

class TxInput: public OutPoint {
public:
    std::string address;

    /// ADA amount
    Amount amount;

    /// Token amounts (optional)
    TokenBundle tokenBundle;

    static TxInput fromProto(const Proto::TxInput& proto);
    Proto::TxInput toProto() const;
};

bool operator==(const TxInput& i1, const TxInput& i2);

class TxOutput {
public:
    Data address;

    /// ADA amount
    Amount amount;

    /// Token amounts (optional)
    TokenBundle tokenBundle;
};

class TransactionPlan {
public:
    std::vector<TxInput> utxos;
    Amount availableAmount = 0;  // total coins in the utxos
    Amount amount = 0;  // coins in the output UTXO
    Amount fee = 0;  // coin amount deducted as fee
    Amount change = 0;  // coins in the change UTXO
    TokenBundle availableTokens;  // total tokens in the utxos (optional)
    TokenBundle outputTokens;  // tokens in the output (optional)
    TokenBundle changeTokens;  // tokens in the change (optional)
    Common::Proto::SigningError error = Common::Proto::SigningError::OK;

    static TransactionPlan fromProto(const Proto::TransactionPlan& proto);
    Proto::TransactionPlan toProto() const;
};

class Transaction {
public:
    std::vector<OutPoint> inputs;
    std::vector<TxOutput> outputs;
    Amount fee;
    uint64_t ttl;

    // Encode into CBOR binary format
    Data encode() const;

    // Derive Transaction ID from hashed encoded data
    Data getId() const;
};

} // namespace TW::Cardano
