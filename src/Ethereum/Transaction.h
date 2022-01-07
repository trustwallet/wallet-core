// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../uint256.h"

namespace TW::Ethereum {

//class Transaction {
//  public:
//    uint256_t nonce;
//    uint256_t gasPrice;
//    uint256_t gasLimit;
//    // Public key hash (Address.bytes)
//    Data to;
//    uint256_t amount;
//    Data payload;
//
//    // Signature values
//    uint256_t v = uint256_t();
//    uint256_t r = uint256_t();
//    uint256_t s = uint256_t();
//
//    Transaction(uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit, Data to, uint256_t amount,
//                Data payload)
//        : nonce(std::move(nonce))
//        , gasPrice(std::move(gasPrice))
//        , gasLimit(std::move(gasLimit))
//        , to(std::move(to))
//        , amount(std::move(amount))
//        , payload(std::move(payload)) {}
//};


/// R-S-V Signature values
struct Signature {
  public:
    uint256_t r, s, v;
};

/// Base class for all transactions.
/// Non-typed and various typed transactions derive from this.
class TransactionBase {
  public:
    uint256_t nonce;
    Data payload;

  public:
    TransactionBase(const uint256_t& nonce, const Data& payload): nonce(nonce), payload(payload) {}
    virtual ~TransactionBase() {}
    // pre-sign hash of the tx, for signing
    virtual Data preHash(const uint256_t &chainID) const = 0;
    // encoded tx (signed)
    virtual Data encoded(const Signature& signature, const uint256_t &chainID) const = 0;
    // encoded tx (unsigned)
    virtual Data serialize(const uint256_t &chainID) const = 0;
    // Signals wether this tx type uses Eip155-style replay protection in the signature
    virtual bool usesReplayProtection() const = 0;

  protected:
    TransactionBase() {}
};

/// Original transaction format, with no explicit type, legacy as pre-EIP2718
class TransactionNonTyped: public TransactionBase {
  public:
    uint256_t gasPrice;
    uint256_t gasLimit;
    // Public key hash (Address.bytes)
    Data to;
    uint256_t amount;

    // Factory methods
    // Create a native transfer transaction
    static std::shared_ptr<TransactionNonTyped> buildNativeTransfer(const uint256_t& nonce,
                                                                    const uint256_t& gasPrice, const uint256_t& gasLimit,
                                                                    const Data& toAddress, const uint256_t& amount, const Data& data = {});

    virtual Data preHash(const uint256_t &chainID) const;
    virtual Data encoded(const Signature& signature, const uint256_t &chainID) const;
    virtual Data serialize(const uint256_t &chainID) const;
    virtual bool usesReplayProtection() const { return true; }

  public:
    TransactionNonTyped(const uint256_t& nonce,
                        const uint256_t& gasPrice, const uint256_t& gasLimit,
                        const Data& to, const uint256_t& amount, const Data& payload = {})
        : TransactionBase(nonce, payload)
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount)) {}
};

enum TransactionType: uint8_t {
    TxType_OptionalAccessList = 0x01,
    TxType_Eip1559 = 0x02,
};

/// EIP1559 transaction
class TransactionEip1559: public TransactionBase {
  public:
    uint256_t maxInclusionFeePerGas;
    uint256_t maxFeePerGas;
    uint256_t gasLimit;
    // Public key hash (Address.bytes)
    Data to;
    uint256_t amount;
    TransactionType txType;

    // Factory methods
    // Create a native transfer transaction
    static std::shared_ptr<TransactionEip1559> buildNativeTransfer(const uint256_t& nonce,
                                                                   const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
                                                                   const Data& toAddress, const uint256_t& amount, const Data& data = {});

    virtual Data preHash(const uint256_t &chainID) const;
    virtual Data encoded(const Signature& signature, const uint256_t &chainID) const;
    virtual Data serialize(const uint256_t &chainID) const;
    virtual bool usesReplayProtection() const { return false; }

  public:
    TransactionEip1559(const uint256_t& nonce,
                       const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasLimit,
                       const Data& to, const uint256_t& amount, const Data& payload = {})
        : TransactionBase(nonce, payload)
        , maxInclusionFeePerGas(std::move(maxInclusionFeePerGas))
        , maxFeePerGas(std::move(maxFeePerGas))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount))
        , txType(TxType_Eip1559) {}


};

} // namespace TW::Ethereum



