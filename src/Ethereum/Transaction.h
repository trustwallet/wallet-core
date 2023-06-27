// Copyright © 2017-2023 Trust Wallet.
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
// - Non-typed (legacy, pre-EIP2718) transactions:
//  -- simple ETH transfer
//  -- others with payload, function call, e.g. ERC20 transfer
// - Typed transactions (enveloped, EIP2718), with specific type and transaction payload

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
    virtual Data preHash(const uint256_t chainID) const = 0;
    // pre-sign image of tx
    virtual Data serialize(const uint256_t chainID) const = 0;
    // encoded tx (signed)
    virtual Data encoded(const Signature& signature, const uint256_t chainID) const = 0;
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

    // Create an ERC20 token transfer transaction
    static std::shared_ptr<TransactionNonTyped> buildERC20Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& toAddress, const uint256_t& amount);

    // Create an ERC20 approve transaction
    static std::shared_ptr<TransactionNonTyped> buildERC20Approve(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount);

    // Create an ERC721 NFT transfer transaction
    static std::shared_ptr<TransactionNonTyped> buildERC721Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId);

    // Create an ERC1155 NFT transfer transaction
    static std::shared_ptr<TransactionNonTyped> buildERC1155Transfer(const uint256_t& nonce,
        const uint256_t& gasPrice, const uint256_t& gasLimit,
        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data);

    // Helpers for building contract calls
    static Data buildERC20TransferCall(const Data& to, const uint256_t& amount);
    static Data buildERC20ApproveCall(const Data& spender, const uint256_t& amount);
    static Data buildERC721TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId);
    static Data buildERC1155TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data);

    virtual Data preHash(const uint256_t chainID) const;
    virtual Data serialize(const uint256_t chainID) const;
    virtual Data encoded(const Signature& signature, const uint256_t chainID) const;
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
    TxType_Eip4337 = 0x03,
};

/// Base class for various typed transactions.
class TransactionTyped: public TransactionBase {
public:
    // transaction type
    TransactionType type;

    TransactionTyped(TransactionType type, const uint256_t& nonce, const Data& payload)
    : TransactionBase(nonce, payload), type(type) {}
    virtual bool usesReplayProtection() const { return false; }
};

/// EIP1559 transaction
class TransactionEip1559: public TransactionTyped {
public:
    uint256_t maxInclusionFeePerGas;
    uint256_t maxFeePerGas;
    uint256_t gasLimit;
    // Public key hash (Address.bytes)
    Data to;
    uint256_t amount;

    // Factory methods
    // Create a native transfer transaction
    static std::shared_ptr<TransactionEip1559> buildNativeTransfer(const uint256_t& nonce,
        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
        const Data& toAddress, const uint256_t& amount, const Data& data = {});
    // Create an ERC20 token transfer transaction
    static std::shared_ptr<TransactionEip1559> buildERC20Transfer(const uint256_t& nonce,
        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
        const Data& tokenContract, const Data& toAddress, const uint256_t& amount);
    // Create an ERC20 approve transaction
    static std::shared_ptr<TransactionEip1559> buildERC20Approve(const uint256_t& nonce,
        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
        const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount);
    // Create an ERC721 NFT transfer transaction
    static std::shared_ptr<TransactionEip1559> buildERC721Transfer(const uint256_t& nonce,
        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId);
    // Create an ERC1155 NFT transfer transaction
    static std::shared_ptr<TransactionEip1559> buildERC1155Transfer(const uint256_t& nonce,
        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data);

    virtual Data preHash(const uint256_t chainID) const;
    virtual Data serialize(const uint256_t chainID) const;
    virtual Data encoded(const Signature& signature, const uint256_t chainID) const;

public:
    TransactionEip1559(const uint256_t& nonce,
        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasLimit,
        const Data& to, const uint256_t& amount, const Data& payload = {})
        : TransactionTyped(TxType_Eip1559, nonce, payload)
        , maxInclusionFeePerGas(std::move(maxInclusionFeePerGas))
        , maxFeePerGas(std::move(maxFeePerGas))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount)) {}
};

/// EIP4337 UserOperation
// https://github.com/ethereum/EIPs/blob/3fd65b1a782912bfc18cb975c62c55f733c7c96e/EIPS/eip-4337.md#specification
class UserOperation: public TransactionTyped {
    using UserOperationPtr = std::shared_ptr<UserOperation>;
public:
    Data entryPoint;
    Data sender;
    Data initCode;
    uint256_t gasLimit;
    uint256_t verificationGasLimit;
    uint256_t maxFeePerGas;
    uint256_t maxInclusionFeePerGas;
    uint256_t preVerificationGas;
    Data paymasterAndData;

    // Factory methods
    // Create a native transfer transaction
    static UserOperationPtr buildNativeTransfer(const Data& entryPointAddress, const Data& senderAddress,
                                                const Data& toAddress, const uint256_t& amount, const uint256_t& nonce,
                                                const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                                const Data& paymasterAndData = {}, const Data& initCode = {}, const Data& payload = {});

    // Create a batched transaction for ERC-4337 wallets
    static UserOperationPtr buildBatch(const Data& entryPointAddress, const Data& senderAddress,
                                       const std::vector<Data>& toAddresses, const std::vector<uint256_t>& amounts, const uint256_t& nonce,
                                       const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                       const Data paymasterAndData = {}, const Data& initCode = {}, const std::vector<Data>& payloads = {});

    // Create an ERC20 token transfer transaction
    static UserOperationPtr buildERC20Transfer(const Data& entryPointAddress, const Data& senderAddress,
                                               const Data& tokenContract, const Data& toAddress, const uint256_t& amount, const uint256_t& nonce,
                                               const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                               const Data& paymasterAndData = {}, const Data& initCode = {});
    // Create an ERC20 approve transaction
    static UserOperationPtr buildERC20Approve(const Data& entryPointAddress, const Data& senderAddress,
                                              const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount, const uint256_t& nonce,
                                              const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                              const Data& paymasterAndData = {}, const Data& initCode = {});
    // Create an ERC721 NFT transfer transaction
    static UserOperationPtr buildERC721Transfer(const Data& entryPointAddress, const Data& senderAddress,
                                                const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& nonce,
                                                const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                                const Data& paymasterAndData = {}, const Data& initCode = {});
    // Create an ERC1155 NFT transfer transaction
    static UserOperationPtr buildERC1155Transfer(const Data& entryPointAddress, const Data& senderAddress,
                                                 const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data, const uint256_t& nonce,
                                                 const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                                 const Data& paymasterAndData = {}, const Data& initCode = {});

    virtual Data preHash(const uint256_t chainID) const;
    virtual Data serialize(const uint256_t chainID) const;
    virtual Data encoded(const Signature& signature, const uint256_t chainID) const;

public:
    UserOperation(const Data& entryPoint, const Data& sender, const uint256_t& nonce, const Data& initCode,
                  const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                  const Data& payload = {}, const Data& paymasterAndData = {})
        : TransactionTyped(TxType_Eip4337, nonce, payload)
        , entryPoint(std::move(entryPoint))
        , sender(std::move(sender))
        , initCode(std::move(initCode))
        , gasLimit(std::move(gasLimit))
        , verificationGasLimit(std::move(verificationGasLimit))
        , maxFeePerGas(std::move(maxFeePerGas))
        , maxInclusionFeePerGas(std::move(maxInclusionFeePerGas))
        , preVerificationGas(std::move(preVerificationGas))
        , paymasterAndData(std::move(paymasterAndData)) {}
};

} // namespace TW::Ethereum
