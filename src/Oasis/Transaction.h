// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <utility>

#include "Address.h"
#include "../Cbor.h"
#include "../uint256.h"

namespace TW::Oasis {

// Transfer transaction
class Transaction {
  public:
    // Recipient address
    Address to;
    // Method name
    std::string method;
    // Gas price
    uint64_t gasPrice;
    // Gas amount
    uint256_t gasAmount;
    // Transaction nonce
    uint64_t nonce;
    // Transaction amount
    uint256_t amount;
    // Transaction context
    std::string context;

    Transaction(Address to, std::string method, uint64_t gasPrice, uint256_t gasAmount,
                uint256_t amount, uint64_t nonce, std::string context)
        : to(std::move(to))
        , method(std::move(method))
        , gasPrice(gasPrice)
        , gasAmount(std::move(gasAmount))
        , nonce(nonce)
        , amount(std::move(amount))
        , context(std::move(context)){}

  public:
    // message returns the CBOR encoding of the Message to be signed.
    Cbor::Encode encodeMessage() const;

    // serialize returns the CBOR encoding of the SignedMessage.
    Data serialize(const Data& signature, const PublicKey& publicKey) const;

    // serialize transaction that can be signed
    Data signaturePreimage() const;
};

// Escrow transaction
class Escrow {
  public:
    // Method name
    std::string method;
    // Gas price
    uint64_t gasPrice;
    // Gas amount
    uint256_t gasAmount;
    // Transaction nonce
    uint64_t nonce;
    Address account;
    // Transaction amount
    uint256_t amount;
    // Transaction context
    std::string context;

    Escrow(std::string method, uint64_t gasPrice, uint256_t gasAmount, uint64_t nonce,
            Address account, uint256_t amount, std::string context)
        : method(std::move(method))
        , gasPrice(gasPrice)
        , gasAmount(std::move(gasAmount))
        , nonce(nonce)
        , account(std::move(account))
        , amount(std::move(amount))
        , context(std::move(context)){}

  public:
    // message returns the CBOR encoding of the Message to be signed.
    Cbor::Encode encodeMessage() const;

    // serialize returns the CBOR encoding of the SignedMessage.
    Data serialize(const Data& signature, const PublicKey& publicKey) const;

    // serialize transaction that can be signed
    Data signaturePreimage() const;
};

// ReclaimEscrow transaction
class ReclaimEscrow {
  public:
    // Method name
    std::string method;
    // Gas price
    uint64_t gasPrice;
    // Gas amount
    uint256_t gasAmount;
    // Transaction nonce
    uint64_t nonce;
    Address account;
    // Transaction amount
    uint256_t shares;
    // Transaction context
    std::string context;

    ReclaimEscrow(std::string method, uint64_t gasPrice, uint256_t gasAmount, uint64_t nonce,
            Address account, uint256_t shares, std::string context)
        : method(std::move(method))
        , gasPrice(gasPrice)
        , gasAmount(std::move(gasAmount))
        , nonce(nonce)
        , account(std::move(account))
        , shares(std::move(shares))
        , context(std::move(context)){}

  public:
    // message returns the CBOR encoding of the Message to be signed.
    Cbor::Encode encodeMessage() const;

    // serialize returns the CBOR encoding of the SignedMessage.
    Data serialize(const Data& signature, const PublicKey& publicKey) const;

    // serialize transaction that can be signed
    Data signaturePreimage() const;
};

} // namespace TW::Oasis
