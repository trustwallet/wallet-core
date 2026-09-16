// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "PublicKey.h"
#include "SigData.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Ontology {

class Transaction {

  private:
    uint8_t version = 0;

    uint8_t txType = 0;

    uint32_t nonce = 0;

    uint64_t gasPrice = 0;

    uint64_t gasLimit = 0;

    std::string payer;

    std::vector<uint8_t> payload;

    std::vector<uint8_t> attributes;

    static const std::string ZERO_PAYER;

  public:
    static const size_t sigVecLimit = 16;

    std::vector<SigData> sigVec;

    Transaction() = default;
    
    Transaction(uint8_t ver, uint8_t type, uint32_t nonce, uint64_t gasPrice, uint64_t gasLimit,
                std::string payer, std::vector<uint8_t> payload)
        : version(ver)
        , txType(type)
        , nonce(nonce)
        , gasPrice(gasPrice)
        , gasLimit(gasLimit)
        , payload(std::move(payload)) {
        if (payer.empty()) {
            payer = ZERO_PAYER;
        }
        this->payer = payer;
    }

    std::vector<uint8_t> serializeUnsigned();

    std::vector<uint8_t> serialize();

    std::vector<uint8_t> txHash();

    std::vector<uint8_t> serialize(const PublicKey& pk);
};

} // namespace TW::Ontology