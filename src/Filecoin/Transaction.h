// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <utility>

#include "Address.h"
#include "../Cbor.h"
#include "../uint256.h"

namespace TW::Filecoin {

Data encodeBigInt(const uint256_t& value);

class Transaction {
  public:
    enum class MethodType: uint64_t {
        /// Simple transfers.
        SEND = 0,
        /// InvokeEVM method.
        INVOKE_EVM = 3844450837,
    };

    enum class SignatureType: uint8_t {
        SECP256K1 = 1,
        DELEGATED = 3,
    };

    // Transaction version
    uint64_t version;
    // Recipient address
    Address to;
    // Sender address
    Address from;
    // Transaction nonce
    uint64_t nonce;
    // Transaction value
    uint256_t value;
    // Fee settings
    int64_t gasLimit;
    uint256_t gasFeeCap;
    uint256_t gasPremium;
    // Transaction type
    uint64_t method;
    // Transaction data
    Data params;

    Transaction(Address to, Address from, uint64_t nonce, uint256_t value, int64_t gasLimit,
                uint256_t gasFeeCap, uint256_t gasPremium, MethodType method, Data params)
        : version(0)
        , to(std::move(to))
        , from(std::move(from))
        , nonce(nonce)
        , value(std::move(value))
        , gasLimit(gasLimit)
        , gasFeeCap(std::move(gasFeeCap))
        , gasPremium(std::move(gasPremium))
        , method(static_cast<uint64_t>(method))
        , params(std::move(params)) {}

  public:
    // message returns the CBOR encoding of the Filecoin Message to be signed.
    Cbor::Encode message() const;

    // cid returns the raw Filecoin message CID (excluding the signature).
    Data cid() const;

    // serialize returns json ready for MpoolPush rpc
    std::string serialize(SignatureType signatureType, const Data& signature) const;
};

} // namespace TW::Filecoin
