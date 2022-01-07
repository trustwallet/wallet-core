// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "Transaction.h"
#include "ABI/Function.h"
#include "ABI/ParamBase.h"
#include "ABI/ParamAddress.h"
#include "RLP.h"
#include "HexCoding.h"

using namespace TW::Ethereum::ABI;
using namespace TW::Ethereum;
using namespace TW;


static const Data EmptyListEncoded = parse_hex("c0");

std::shared_ptr<TransactionNonTyped> TransactionNonTyped::buildNativeTransfer(const uint256_t& nonce,
                                                                              const uint256_t& gasPrice, const uint256_t& gasLimit,
                                                                              const Data& toAddress, const uint256_t& amount, const Data& data) {
    return std::make_shared<TransactionNonTyped>(nonce, gasPrice, gasLimit, toAddress, amount, data);
}

Data TransactionNonTyped::preHash(const uint256_t &chainID) const {
    return Hash::keccak256(this->serialize(chainID));
}

Data TransactionNonTyped::serialize(const uint256_t &chainID) const {
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

Data TransactionNonTyped::encoded(const Signature& signature, const uint256_t &chainID) const {
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



std::shared_ptr<TransactionEip1559> TransactionEip1559::buildNativeTransfer(const uint256_t& nonce,
                                                                            const uint256_t& maxInclusionFeePerGas, const uint256_t& maxFeePerGas, const uint256_t& gasPrice,
                                                                            const Data& toAddress, const uint256_t& amount, const Data& data) {
    return std::make_shared<TransactionEip1559>(nonce, maxInclusionFeePerGas, maxFeePerGas, gasPrice, toAddress, amount, data);
}

Data TransactionEip1559::preHash(const uint256_t &chainID) const {
    return Hash::keccak256(this->serialize(chainID));
}

Data TransactionEip1559::serialize(const uint256_t &chainID) const {
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
    append(envelope, static_cast<uint8_t>(txType));
    append(envelope, RLP::encodeList(encoded));

    return envelope;
}

Data TransactionEip1559::encoded(const Signature& signature, const uint256_t &chainID) const {
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
    append(envelope, static_cast<uint8_t>(txType));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}


