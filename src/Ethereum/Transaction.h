// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../uint256.h"

namespace TW::Ethereum {

// Transactions can be:
// - Legacy transactions: (pre-EIP2718)
//  -- simple ETH transfer
//  -- others with payload, function call, e.g. ERC20 transfer
// - Enveloped transactions (EIP2718), with specific type, and specific transaction payload

enum TransactionType: uint8_t {
    OptionalAccessList = 0x01,
    Legacy = 0xee, // TODO
};

/// Original transaction format, legacy as pre-EIP2718
class TransactionLegacy {
public:
    uint256_t nonce;
    uint256_t gasPrice;
    uint256_t gasLimit;
    // Public key hash (Address.bytes)
    Data to;
    uint256_t amount;
    Data payload;

    // Signature values
    uint256_t v = uint256_t();
    uint256_t r = uint256_t();
    uint256_t s = uint256_t();

    // Factory methods
    // Create an ERC20 token transfer transaction
    static TransactionLegacy buildERC20Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& toAddress, const uint256_t& amount);

    // Create an ERC20 approve transaction
    static TransactionLegacy buildERC20Approve(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount);

    // Create an ERC721 NFT transfer transaction
    static TransactionLegacy buildERC721Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId);

    // Create an ERC1155 NFT transfer transaction
    static TransactionLegacy buildERC1155Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data);

public:
    TransactionLegacy(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& to, const uint256_t& amount, const Data& payload = {})
        : nonce(std::move(nonce))
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount))
        , payload(std::move(payload)) {}
};

class TransactionLegacyPayload {
public:
    // Helpers for building contract calls
    static Data buildERC20TransferCall(const Data& to, const uint256_t& amount);
    static Data buildERC20ApproveCall(const Data& spender, const uint256_t& amount);
    static Data buildERC721TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId);
    static Data buildERC1155TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data);
};

class TransactionEnveloped {
public:
    static Data buildERC2930AccessListPayload(uint256_t nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& to,
        const uint256_t& value,
        const Data& data,
        const Data& accessList,
        const Data& yParity,
        const Data& senderR,
        const Data& senderS);
};

/// Can be legacy, or enveloped with a type (EIP2718)
class Transaction {
public:
    // legacy or enveloped
    bool isLegacy;
    // transaction type (for enveloped)
    TransactionType type;
    // the encoded legacy transaction OR transaction payload of the enveloped transactions
    Data encoded;

   static Transaction createLegacy(const Data& legacyEncoded);
   static Transaction createEnveloped(TransactionType type, const Data& payloadEncoded);

protected:
    // generic ctor
    Transaction(bool isLegacy, TransactionType type, const Data& encoded)
        : isLegacy(isLegacy)
        , type(type)
        , encoded(std::move(encoded))
        {}
};

} // namespace TW::Ethereum
