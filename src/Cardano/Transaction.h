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

    TokenAmount() = default;
    TokenAmount(const std::string& policyId, const std::string& assetName, uint256_t amount) : policyId(policyId), assetName(assetName), amount(amount) {}

    static TokenAmount fromProto(const Proto::TokenAmount& proto);
    Proto::TokenAmount toProto() const;
    std::string key() const { return policyId + "_" + assetName; }
};

class TokenBundle {
public:
    std::map<std::string, TokenAmount> bundle;

    TokenBundle() = default;
    TokenBundle(const std::vector<TokenAmount>& tokens) { for (const auto& t: tokens) { add(t); } }

    static TokenBundle fromProto(const Proto::TokenBundle& proto);
    Proto::TokenBundle toProto() const;

    void add(const TokenAmount& ta);
    uint256_t getAmount(const std::string& key) const;
    size_t size() const { return bundle.size(); }

    // The minimum ADA amount needed for an ADA-only UTXO
    static const uint64_t MinUtxoValue;
    // The minimum ADA amount needed for a UTXO with this token bundle.  See https://docs.cardano.org/native-tokens/minimum-ada-value-requirement
    uint64_t minAdaAmount() const;
    static uint64_t minAdaAmountHelper(uint64_t numPids, uint64_t numAssets, uint64_t sumAssetNameLengths);
};

class OutPoint {
public:
    Data txHash;
    uint64_t outputIndex;

    OutPoint() = default;
    OutPoint(const Data& txHash, uint64_t outputIndex) : txHash(txHash), outputIndex(outputIndex) {}
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

    TxOutput() = default;
    TxOutput(const Data& address, Amount amount) : address(address), amount(amount) {}
    TxOutput(const Data& address, Amount amount, const TokenBundle& tokenBundle) : address(address), amount(amount), tokenBundle(tokenBundle) {}
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
