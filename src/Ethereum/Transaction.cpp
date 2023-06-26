// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "Ethereum/ABI.h"
#include "HexCoding.h"
#include "RLP.h"
#include "Signer.h"
#include <Ethereum/ERC4337.h>
#include <Ethereum/MessageSigner.h>
#include <nlohmann/json.hpp>

namespace TW::Ethereum {

using json = nlohmann::json;
using ParamBasePtr = std::shared_ptr<ABI::ParamBase>;
using ParamCollection = std::vector<ParamBasePtr>;
using UserOperationPtr = std::shared_ptr<UserOperation>;

static const Data EmptyListEncoded = parse_hex("c0");

/// TransactionNonTyped
std::shared_ptr<TransactionNonTyped>
TransactionNonTyped::buildNativeTransfer(const uint256_t& nonce,
                                         const uint256_t& gasPrice, const uint256_t& gasLimit,
                                         const Data& toAddress, const uint256_t& amount, const Data& data) {
    return std::make_shared<TransactionNonTyped>(nonce, gasPrice, gasLimit, toAddress, amount, data);
}

std::shared_ptr<TransactionNonTyped>
TransactionNonTyped::buildERC20Transfer(const uint256_t& nonce,
                                        const uint256_t& gasPrice, const uint256_t& gasLimit,
                                        const Data& tokenContract, const Data& toAddress, const uint256_t& amount) {
    return std::make_shared<TransactionNonTyped>(nonce, gasPrice, gasLimit, tokenContract, 0, buildERC20TransferCall(toAddress, amount));
}

std::shared_ptr<TransactionNonTyped>
TransactionNonTyped::buildERC20Approve(const uint256_t& nonce,
                                       const uint256_t& gasPrice, const uint256_t& gasLimit,
                                       const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount) {
    return std::make_shared<TransactionNonTyped>(nonce, gasPrice, gasLimit, tokenContract, 0, buildERC20ApproveCall(spenderAddress, amount));
}

std::shared_ptr<TransactionNonTyped>
TransactionNonTyped::buildERC721Transfer(const uint256_t& nonce,
                                         const uint256_t& gasPrice, const uint256_t& gasLimit,
                                         const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId) {
    return std::make_shared<TransactionNonTyped>(nonce, gasPrice, gasLimit, tokenContract, 0, buildERC721TransferFromCall(from, to, tokenId));
}

std::shared_ptr<TransactionNonTyped>
TransactionNonTyped::buildERC1155Transfer(const uint256_t& nonce,
                                          const uint256_t& gasPrice, const uint256_t& gasLimit,
                                          const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data) {
    return std::make_shared<TransactionNonTyped>(nonce, gasPrice, gasLimit, tokenContract, 0, buildERC1155TransferFromCall(from, to, tokenId, value, data));
}

Data TransactionNonTyped::preHash(const uint256_t chainID) const {
    return Hash::keccak256(serialize(chainID));
}

Data TransactionNonTyped::serialize(const uint256_t chainID) const {
    Data encoded;
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(0));
    append(encoded, RLP::encode(0));
    return RLP::encodeList(encoded);
}

Data TransactionNonTyped::encoded(const Signature& signature, [[maybe_unused]] const uint256_t chainID) const {
    Data encoded;
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    append(encoded, RLP::encode(signature.v));
    append(encoded, RLP::encode(signature.r));
    append(encoded, RLP::encode(signature.s));
    return RLP::encodeList(encoded);
}

Data TransactionNonTyped::buildERC20TransferCall(const Data& to, const uint256_t& amount) {
    // clang-format off
    auto func = ABI::Function("transfer", ParamCollection{
        std::make_shared<ABI::ParamAddress>(to),
        std::make_shared<ABI::ParamUInt256>(amount)
    });
    // clang-format on
    Data payload;
    func.encode(payload);
    return payload;
}

Data TransactionNonTyped::buildERC20ApproveCall(const Data& spender, const uint256_t& amount) {
    // clang-format off
    auto func = ABI::Function("approve", ParamCollection{
        std::make_shared<ABI::ParamAddress>(spender),
        std::make_shared<ABI::ParamUInt256>(amount)
    });
    // clang-format on
    Data payload;
    func.encode(payload);
    return payload;
}

Data TransactionNonTyped::buildERC721TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId) {
    // clang-format off
    auto func = ABI::Function("transferFrom", ParamCollection{
        std::make_shared<ABI::ParamAddress>(from),
        std::make_shared<ABI::ParamAddress>(to),
        std::make_shared<ABI::ParamUInt256>(tokenId)
    });
    // clang-format on
    Data payload;
    func.encode(payload);
    return payload;
}

Data TransactionNonTyped::buildERC1155TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data) {
    // clang-format off
    auto func = ABI::Function("safeTransferFrom", ParamCollection{
        std::make_shared<ABI::ParamAddress>(from),
        std::make_shared<ABI::ParamAddress>(to),
        std::make_shared<ABI::ParamUInt256>(tokenId),
        std::make_shared<ABI::ParamUInt256>(value),
        std::make_shared<ABI::ParamByteArray>(data)
    });
    // clang-format on
    Data payload;
    func.encode(payload);
    return payload;
}

/// TransactionEip1559
Data TransactionEip1559::preHash(const uint256_t chainID) const {
    return Hash::keccak256(serialize(chainID));
}

Data TransactionEip1559::serialize(const uint256_t chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(maxInclusionFeePerGas));
    append(encoded, RLP::encode(maxFeePerGas));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    append(encoded, EmptyListEncoded); // empty accessList

    Data envelope;
    append(envelope, static_cast<uint8_t>(type));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}

Data TransactionEip1559::encoded(const Signature& signature, const uint256_t chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(maxInclusionFeePerGas));
    append(encoded, RLP::encode(maxFeePerGas));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    append(encoded, EmptyListEncoded); // empty accessList
    append(encoded, RLP::encode(signature.v));
    append(encoded, RLP::encode(signature.r));
    append(encoded, RLP::encode(signature.s));

    Data envelope;
    append(envelope, static_cast<uint8_t>(type));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}

std::shared_ptr<TransactionEip1559>
TransactionEip1559::buildNativeTransfer(const uint256_t& nonce,
                                        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
                                        const Data& toAddress, const uint256_t& amount, const Data& data) {
    return std::make_shared<TransactionEip1559>(nonce, maxInclusionFeePerGas, maxFeePerGas, gasPrice, toAddress, amount, data);
}

std::shared_ptr<TransactionEip1559>
TransactionEip1559::buildERC20Transfer(const uint256_t& nonce,
                                       const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
                                       const Data& tokenContract, const Data& toAddress, const uint256_t& amount) {
    return std::make_shared<TransactionEip1559>(nonce, maxInclusionFeePerGas, maxFeePerGas, gasPrice, tokenContract, 0, TransactionNonTyped::buildERC20TransferCall(toAddress, amount));
}

std::shared_ptr<TransactionEip1559>
TransactionEip1559::buildERC20Approve(const uint256_t& nonce,
                                      const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
                                      const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount) {
    return std::make_shared<TransactionEip1559>(nonce, maxInclusionFeePerGas, maxFeePerGas, gasPrice, tokenContract, 0, TransactionNonTyped::buildERC20ApproveCall(spenderAddress, amount));
}

std::shared_ptr<TransactionEip1559>
TransactionEip1559::buildERC721Transfer(const uint256_t& nonce,
                                        const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
                                        const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId) {
    return std::make_shared<TransactionEip1559>(nonce, maxInclusionFeePerGas, maxFeePerGas, gasPrice, tokenContract, 0, TransactionNonTyped::buildERC721TransferFromCall(from, to, tokenId));
}

std::shared_ptr<TransactionEip1559>
TransactionEip1559::buildERC1155Transfer(const uint256_t& nonce,
                                         const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
                                         const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data) {
    return std::make_shared<TransactionEip1559>(nonce, maxInclusionFeePerGas, maxFeePerGas, gasPrice, tokenContract, 0, TransactionNonTyped::buildERC1155TransferFromCall(from, to, tokenId, value, data));
}

/// UserOperation
Data UserOperation::preHash(const uint256_t chainID) const {
    auto params = ABI::ParamTuple(ParamCollection{
        std::make_shared<ABI::ParamByteArrayFix>(32, Hash::keccak256(serialize(chainID))),
        std::make_shared<ABI::ParamAddress>(entryPoint),
        std::make_shared<ABI::ParamUInt256>(chainID)});
    Data encoded;
    params.encode(encoded);
    return Hash::keccak256(encoded);
}

Data UserOperation::serialize([[maybe_unused]] const uint256_t chainID) const {
    auto params = ABI::ParamTuple(ParamCollection{
        std::make_shared<ABI::ParamAddress>(sender),
        std::make_shared<ABI::ParamUInt256>(nonce),
        std::make_shared<ABI::ParamByteArrayFix>(32, Hash::keccak256(initCode)),
        std::make_shared<ABI::ParamByteArrayFix>(32, Hash::keccak256(payload)),
        std::make_shared<ABI::ParamUInt256>(gasLimit),
        std::make_shared<ABI::ParamUInt256>(verificationGasLimit),
        std::make_shared<ABI::ParamUInt256>(preVerificationGas),
        std::make_shared<ABI::ParamUInt256>(maxFeePerGas),
        std::make_shared<ABI::ParamUInt256>(maxInclusionFeePerGas),
        std::make_shared<ABI::ParamByteArrayFix>(32, Hash::keccak256(paymasterAndData))});
    Data serialized;
    params.encode(serialized);

    return serialized;
}

Data UserOperation::encoded(const Signature& signature, [[maybe_unused]] const uint256_t chainID) const {
    Data rawSignature = Signer::simpleStructToSignatureData(signature);
    rawSignature[64] += 27;

    const json tx = {
        {"sender", hexEncoded(sender)},
        {"nonce", nonce.str()},
        {"initCode", hexEncoded(initCode)},
        {"callData", hexEncoded(payload)},
        {"callGasLimit", gasLimit.str()},
        {"verificationGasLimit", verificationGasLimit.str()},
        {"maxFeePerGas", maxFeePerGas.str()},
        {"maxPriorityFeePerGas", maxInclusionFeePerGas.str()},
        {"paymasterAndData", hexEncoded(paymasterAndData)},
        {"preVerificationGas", preVerificationGas.str()},
        {"signature", hexEncoded(rawSignature)}};
    const auto txString = tx.dump();
    return Data(txString.begin(), txString.end());
}

UserOperationPtr
UserOperation::buildNativeTransfer(const Data& entryPointAddress, const Data& senderAddress,
                                   const Data& toAddress, const uint256_t& amount, const uint256_t& nonce,
                                   const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                   const Data& paymasterAndData, const Data& initCode, const Data& payload) {
    return std::make_shared<UserOperation>(
        entryPointAddress,
        senderAddress,
        nonce,
        initCode,
        gasLimit,
        verificationGasLimit,
        maxFeePerGas,
        maxInclusionFeePerGas,
        preVerificationGas,
        Ethereum::getERC4337ExecuteBytecode(toAddress, amount, payload),
        paymasterAndData);
}

UserOperationPtr
UserOperation::buildBatch(const Data& entryPointAddress, const Data& senderAddress,
                          const std::vector<Data>& toAddresses, const std::vector<uint256_t>& amounts, const uint256_t& nonce,
                          const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                          const Data paymasterAndData, const Data& initCode, const std::vector<Data>& payloads) {
    return std::make_shared<UserOperation>(
        entryPointAddress,
        senderAddress,
        nonce,
        initCode,
        gasLimit,
        verificationGasLimit,
        maxFeePerGas,
        maxInclusionFeePerGas,
        preVerificationGas,
        Ethereum::getERC4337ExecuteBatchBytecode(toAddresses, amounts, payloads),
        paymasterAndData);
}

UserOperationPtr
UserOperation::buildERC20Transfer(const Data& entryPointAddress, const Data& senderAddress,
                                  const Data& tokenContract, const Data& toAddress, const uint256_t& amount, const uint256_t& nonce,
                                  const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                  const Data& paymasterAndData, const Data& initCode) {
    return std::make_shared<UserOperation>(
        entryPointAddress,
        senderAddress,
        nonce,
        initCode,
        gasLimit,
        verificationGasLimit,
        maxFeePerGas,
        maxInclusionFeePerGas,
        preVerificationGas,
        Ethereum::getERC4337ExecuteBytecode(tokenContract, 0, TransactionNonTyped::buildERC20TransferCall(toAddress, amount)),
        paymasterAndData);
}

UserOperationPtr
UserOperation::buildERC20Approve(const Data& entryPointAddress, const Data& senderAddress,
                                 const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount, const uint256_t& nonce,
                                 const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                 const Data& paymasterAndData, const Data& initCode) {
    return std::make_shared<UserOperation>(
        entryPointAddress,
        senderAddress,
        nonce,
        initCode,
        gasLimit,
        verificationGasLimit,
        maxFeePerGas,
        maxInclusionFeePerGas,
        preVerificationGas,
        Ethereum::getERC4337ExecuteBytecode(tokenContract, 0, TransactionNonTyped::buildERC20ApproveCall(spenderAddress, amount)),
        paymasterAndData);
}

UserOperationPtr
UserOperation::buildERC721Transfer(const Data& entryPointAddress, const Data& senderAddress,
                                   const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& nonce,
                                   const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                   const Data& paymasterAndData, const Data& initCode) {
    return std::make_shared<UserOperation>(
        entryPointAddress,
        senderAddress,
        nonce,
        initCode,
        gasLimit,
        verificationGasLimit,
        maxFeePerGas,
        maxInclusionFeePerGas,
        preVerificationGas,
        Ethereum::getERC4337ExecuteBytecode(tokenContract, 0, TransactionNonTyped::buildERC721TransferFromCall(from, to, tokenId)),
        paymasterAndData);
}

UserOperationPtr
UserOperation::buildERC1155Transfer(const Data& entryPointAddress, const Data& senderAddress,
                                    const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data, const uint256_t& nonce,
                                    const uint256_t& gasLimit, const uint256_t& verificationGasLimit, const uint256_t& maxFeePerGas, const uint256_t& maxInclusionFeePerGas, const uint256_t& preVerificationGas,
                                    const Data& paymasterAndData, const Data& initCode) {
    return std::make_shared<UserOperation>(
        entryPointAddress,
        senderAddress,
        nonce,
        initCode,
        gasLimit,
        verificationGasLimit,
        maxFeePerGas,
        maxInclusionFeePerGas,
        preVerificationGas,
        Ethereum::getERC4337ExecuteBytecode(tokenContract, 0, TransactionNonTyped::buildERC1155TransferFromCall(from, to, tokenId, value, data)),
        paymasterAndData);
}

} // namespace TW::Ethereum