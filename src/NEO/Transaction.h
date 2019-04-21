// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "PublicKey.h"
#include "SigData.h"
#include "../PublicKey.h"

#include <string>

namespace TW::NEO {

class Transaction {

  private:
    uint8_t version;

    uint8_t txType;

    uint32_t nonce;

    uint64_t gasPrice;

    uint64_t gasLimit;

    std::string payer;

    std::vector<uint8_t> payload;

    std::vector<uint8_t> attributes;

    static const std::string ZERO_PAYER;

  public:
    static const size_t sigVecLimit = 16;

    std::vector<SigData> sigVec;

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

    template <std::size_t Size>
    std::array<uint8_t, Size> arrayByteToArrayUint(const std::array<byte, Size> &bytes) const {
        std::array<uint8_t, Size> builderData;
        for (std::size_t i = 0; i < Size; ++i) {
            builderData[i] = (uint8_t) bytes[i];
        }
        return builderData;
    };
};

} // namespace TW::NEO
