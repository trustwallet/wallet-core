// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../uint256.h"

#include <memory>

namespace TW::Ethereum {

// Transactions can be:
// - Legacy transactions: (pre-EIP2718)
//  -- simple ETH transfer
//  -- others with payload, function call, e.g. ERC20 transfer
// - Enveloped transactions (EIP2718), with specific type, and specific transaction payload

struct SignatureRSV {
public:
    uint256_t r;
    uint256_t s;
    uint256_t v;
};

/// Base class for all transactions.
/// Legacy and various enveloped transactions derive from this.
class TransactionBase {
public:
    uint256_t nonce;
    Data payload;

public:
    TransactionBase(const uint256_t& nonce, const Data& payload): nonce(nonce), payload(payload) {}
    virtual ~TransactionBase() {}
    // pre-sign encoded tx, for hashing
    virtual Data hash(const uint256_t chainID) const = 0;
    // encoded tx (signed)
    virtual Data encoded(const SignatureRSV& signature, const uint256_t chainID) const = 0;

protected:
    TransactionBase() {}
};

/// Original transaction format, legacy as pre-EIP2718
class TransactionLegacy: public TransactionBase {
public:
    uint256_t gasPrice;
    uint256_t gasLimit;
    // Public key hash (Address.bytes)
    Data to;
    uint256_t amount;

    // Factory methods
    // Create a native transfer transaction
    static std::shared_ptr<TransactionLegacy> buildNativeTransfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& toAddress, const uint256_t& amount, const Data& data = {});

    // Create an ERC20 token transfer transaction
    static std::shared_ptr<TransactionLegacy> buildERC20Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& toAddress, const uint256_t& amount);

    // Create an ERC20 approve transaction
    static std::shared_ptr<TransactionLegacy> buildERC20Approve(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount);

    // Create an ERC721 NFT transfer transaction
    static std::shared_ptr<TransactionLegacy> buildERC721Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId);

    // Create an ERC1155 NFT transfer transaction
    static std::shared_ptr<TransactionLegacy> buildERC1155Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data);

    // Helpers for building contract calls
    static Data buildERC20TransferCall(const Data& to, const uint256_t& amount);
    static Data buildERC20ApproveCall(const Data& spender, const uint256_t& amount);
    static Data buildERC721TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId);
    static Data buildERC1155TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data);

    virtual Data hash(const uint256_t chainID) const;
    virtual Data encoded(const SignatureRSV& signature, const uint256_t chainID) const;

public:
    TransactionLegacy(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& to, const uint256_t& amount, const Data& payload = {})
        : TransactionBase(nonce, payload)
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount)) {}
};

enum TransactionType: uint8_t {
    OptionalAccessList = 0x01,
};

class TransactionEnveloped: public TransactionBase {
public:
    // transaction type (for enveloped)
    TransactionType type;

    TransactionEnveloped(TransactionType type): type(type) {}
};

} // namespace TW::Ethereum
