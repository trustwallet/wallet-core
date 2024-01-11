// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "../uint256.h"
#include "../proto/Nebulas.pb.h"

namespace TW::Nebulas {

class Transaction {
  public:
    static const char* TxPayloadBinaryType;
    static const char* TxPayloadDeployType;
    static const char* TxPayloadCallType;

    Address from;
    uint256_t nonce;
    uint256_t gasPrice;
    uint256_t gasLimit;
    Address to;
    uint256_t amount;
    uint256_t timestamp;
    std::string payload;

    // Signature values
    uint256_t chainID;
    Data hash;
    Data signature;
    uint32_t algorithm = 0;

    // serialize data
    Data raw;

    Transaction(Address from, uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit, Address to, uint256_t amount, uint256_t timestamp, const std::string& payload)
        : from(std::move(from))
        , nonce(std::move(nonce))
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount))
        , timestamp(std::move(timestamp))
        , payload(payload){}

  public:
    static Proto::Data* newPayloadData(const std::string& payload);

    ///serialize the signed transaction.
    void serializeToRaw();
};

} // namespace TW::Nebulas