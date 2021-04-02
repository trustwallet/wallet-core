// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "ABI/Function.h"
#include "ABI/ParamBase.h"
#include "ABI/ParamAddress.h"
#include "RLP.h"
#include "../HexCoding.h"

using namespace TW::Ethereum::ABI;
using namespace TW::Ethereum;
using namespace TW;


TransactionLegacy TransactionLegacy::buildNativeTransfer(const uint256_t& nonce,
    const uint256_t& gasPrice, const uint256_t& gasLimit,
    const Data& toAddress, const uint256_t& amount, const Data& data) {
    return TransactionLegacy(nonce, gasPrice, gasLimit, toAddress, amount, data);
}

TransactionLegacy TransactionLegacy::buildERC20Transfer(const uint256_t& nonce,
    const uint256_t& gasPrice, const uint256_t& gasLimit,
    const Data& tokenContract, const Data& toAddress, const uint256_t& amount) {
    return TransactionLegacy(nonce, gasPrice, gasLimit, tokenContract, 0, TransactionLegacyPayload::buildERC20TransferCall(toAddress, amount));
}

TransactionLegacy TransactionLegacy::buildERC20Approve(const uint256_t& nonce,
    const uint256_t& gasPrice, const uint256_t& gasLimit,
    const Data& tokenContract, const Data& spenderAddress, const uint256_t& amount) {
    return TransactionLegacy(nonce, gasPrice, gasLimit, tokenContract, 0, TransactionLegacyPayload::buildERC20ApproveCall(spenderAddress, amount));
}

TransactionLegacy TransactionLegacy::buildERC721Transfer(const uint256_t& nonce,
    const uint256_t& gasPrice, const uint256_t& gasLimit,
    const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId) {
    return TransactionLegacy(nonce, gasPrice, gasLimit, tokenContract, 0, TransactionLegacyPayload::buildERC721TransferFromCall(from, to, tokenId));
}

TransactionLegacy TransactionLegacy::buildERC1155Transfer(const uint256_t& nonce,
    const uint256_t& gasPrice, const uint256_t& gasLimit,
    const Data& tokenContract, const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data) {
    return TransactionLegacy(nonce, gasPrice, gasLimit, tokenContract, 0, TransactionLegacyPayload::buildERC1155TransferFromCall(from, to, tokenId, value, data));
}

Data TransactionLegacy::hash(const uint256_t chainID) const {
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
    return Hash::keccak256(RLP::encodeList(encoded));
}

Data TransactionLegacy::encoded(const SignatureRSV& signature, const uint256_t chainID) const {
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

Data TransactionLegacyPayload::buildERC20TransferCall(const Data& to, const uint256_t& amount) {
    auto func = Function("transfer", std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(to),
        std::make_shared<ParamUInt256>(amount)
    });
    Data payload;
    func.encode(payload);
    return payload;
}

Data TransactionLegacyPayload::buildERC20ApproveCall(const Data& spender, const uint256_t& amount) {
    auto func = Function("approve", std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(spender),
        std::make_shared<ParamUInt256>(amount)
    });
    Data payload;
    func.encode(payload);
    return payload;
}

Data TransactionLegacyPayload::buildERC721TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId) {
    auto func = Function("transferFrom", std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(from),
        std::make_shared<ParamAddress>(to),
        std::make_shared<ParamUInt256>(tokenId)
    });
    Data payload;
    func.encode(payload);
    return payload;
}

Data TransactionLegacyPayload::buildERC1155TransferFromCall(const Data& from, const Data& to, const uint256_t& tokenId, const uint256_t& value, const Data& data) {
    auto func = Function("safeTransferFrom", std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(from),
        std::make_shared<ParamAddress>(to),
        std::make_shared<ParamUInt256>(tokenId),
        std::make_shared<ParamUInt256>(value),
        std::make_shared<ParamByteArray>(data)
    });
    Data payload;
    func.encode(payload);
    return payload;
}

/*
TransactionXX TransactionXX::createLegacy(const Data& legacyEncoded) {
    return TransactionXX(true, TransactionType::Legacy, legacyEncoded);
}

TransactionXX TransactionXX::createEnveloped(TransactionType type, const Data& payloadEncoded) {
    uint8_t type8 = static_cast<uint8_t>(type);
    assert(type8 > 0 && type8 <= 0x7f);
    return TransactionXX(false, type, payloadEncoded);
}
*/

Data TransactionAccessList::encodeAccessList(const std::vector<ALItem>& al) {
    Data encoded;
    append(encoded, RLP::encodeList(al));
    std::cout << "accesslist encoded " << hex(encoded) << "\n";
    return encoded;
}

Data TransactionAccessList::hash(const uint256_t chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to));
    append(encoded, RLP::encode(value));
    append(encoded, RLP::encode(data));
    append(encoded, encodeAccessList(accessList));
    Data encoded2;
    append(encoded2, static_cast<uint8_t>(type));
    append(encoded2, RLP::encodeList(encoded));
    return Hash::keccak256(encoded2);
}

Data TransactionAccessList::encoded(const SignatureRSV& signature, const uint256_t chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to));
    append(encoded, RLP::encode(value));
    append(encoded, RLP::encode(data));
    append(encoded, encodeAccessList(accessList));
    append(encoded, RLP::encode(uint8_t(0)));
    append(encoded, RLP::encode(signature.r));
    append(encoded, RLP::encode(signature.s));
    Data encoded2;
    append(encoded2, static_cast<uint8_t>(type));
    append(encoded2, RLP::encodeList(encoded));
    return encoded2;
}

