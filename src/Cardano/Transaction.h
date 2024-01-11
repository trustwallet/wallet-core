// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "AddressV3.h"

#include "Data.h"
#include "uint256.h"
#include "../proto/Cardano.pb.h"
#include "../proto/Common.pb.h"

#include <set>
#include <string>
#include <utility>
#include <vector>
#include <string>
#include <set>

namespace TW::Cardano {

typedef uint64_t Amount;

class TokenAmount {
public:
    std::string policyId;
    std::string assetName;
    uint256_t amount;

    TokenAmount() = default;
    TokenAmount(std::string policyId, std::string assetName, uint256_t amount)
        : policyId(std::move(policyId)), assetName(std::move(assetName)), amount(std::move(amount)) {}

    static TokenAmount fromProto(const Proto::TokenAmount& proto);
    Proto::TokenAmount toProto() const;
    /// Key used in TokenBundle
    std::string key() const { return policyId + "_" + assetName; }
};

class TokenBundle {
public:
    std::map<std::string, TokenAmount> bundle;

    TokenBundle() = default;
    explicit TokenBundle(const std::vector<TokenAmount>& tokens) {
        for (const auto& t : tokens) {
            add(t);
        }
    }

    static TokenBundle fromProto(const Proto::TokenBundle& proto);
    Proto::TokenBundle toProto() const;

    void add(const TokenAmount& ta);
    uint256_t getAmount(const std::string& key) const;
    size_t size() const { return bundle.size(); }
    /// Get the unique policyIds, can be the same number as the elements, or less (in case a policyId appears more than once, with different asset names).
    std::unordered_set<std::string> getPolicyIds() const;
    /// Filter by policyIds
    std::vector<TokenAmount> getByPolicyId(const std::string& policyId) const;

    // The minimum ADA amount needed for an ADA-only UTXO
    static const uint64_t MinUtxoValue;
    // The minimum ADA amount needed for a UTXO with this token bundle.  See https://docs.cardano.org/native-tokens/minimum-ada-value-requirement
    uint64_t minAdaAmount() const;
    static uint64_t minAdaAmountHelper(uint64_t numPids, uint64_t numAssets, uint64_t sumAssetNameLengths);
};

class OutPoint {
public:
    Data txHash;
    uint64_t outputIndex{};

    OutPoint() = default;
    OutPoint(Data txHash, uint64_t outputIndex)
        : txHash(std::move(txHash)), outputIndex(outputIndex) {}
};

class TxInput : public OutPoint {
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
    Amount amount{};

    /// Token amounts (optional)
    TokenBundle tokenBundle;

    /// Returns minimal amount of ADA for the output or `std::nullopt` if there a problem happened.
    std::optional<uint64_t> minAdaAmount(uint64_t coinsPerUtxoByte) const noexcept;

    TxOutput() = default;
    TxOutput(Data address, Amount amount)
        : address(std::move(address)), amount(amount) {}
    TxOutput(Data address, Amount amount, TokenBundle tokenBundle)
        : address(std::move(address)), amount(amount), tokenBundle(std::move(tokenBundle)) {}

    static TxOutput fromProto(const Proto::TxOutput& proto);
    Proto::TxOutput toProto() const;
};

class TransactionPlan {
public:
    std::vector<TxInput> utxos;
    std::vector<TxOutput> extraOutputs;
    Amount availableAmount = 0;  // total coins in the input utxos
    Amount amount = 0;           // coins in the output UTXO
    Amount fee = 0;              // coin amount deducted as fee
    Amount change = 0;           // coins in the change UTXO
    Amount deposit = 0;          // coins deposited (going to deposit) in this TX
    Amount undeposit = 0;        // coins undeposited (returned from deposit) in this TX
    TokenBundle availableTokens; // total tokens in the utxos (optional)
    TokenBundle outputTokens;    // tokens in the output (optional)
    TokenBundle changeTokens;    // tokens in the change (optional)
    Common::Proto::SigningError error = Common::Proto::SigningError::OK;

    static TransactionPlan fromProto(const Proto::TransactionPlan& proto);
    Proto::TransactionPlan toProto() const;
};

/// A key with a type, used in a Certificate
class CertificateKey {
public:
    enum KeyType : uint8_t {
        AddressKeyHash = 0,
        // ScriptHash = 1,
    };
    KeyType type;
    Data key;
};

/// Certificate, mainly used for staking
class Certificate {
public:
    enum CertificateType : uint8_t {
        SkatingKeyRegistration = 0,
        StakingKeyDeregistration = 1,
        Delegation = 2,
        // StakePoolRegistration = 3, // not supported
    };
    CertificateType type;
    CertificateKey certKey;
    /// Optional PoolId, used in delegation
    Data poolId;
};

/// Staking withdrawal
class Withdrawal {
public:
    Data stakingKey;
    Amount amount;
};

class Transaction {
public:
    std::vector<OutPoint> inputs;
    std::vector<TxOutput> outputs;
    Amount fee;
    uint64_t ttl;
    std::vector<Certificate> certificates;
    std::vector<Withdrawal> withdrawals;

    // Encode into CBOR binary format
    Data encode() const;

    // Derive Transaction ID from hashed encoded data
    Data getId() const;
};

} // namespace TW::Cardano
