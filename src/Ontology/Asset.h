// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Data.h"
#include "Signer.h"
#include "Transaction.h"
#include "../BinaryCoding.h"

#include <array>
#include <string>
#include <vector>

namespace TW::Ontology {

class Asset {
protected:
    const uint8_t txType = 0xD1;

public:
    virtual ~Asset() noexcept = default;
    virtual Data contractAddress() = 0;

    virtual Transaction decimals(uint32_t nonce) = 0;

    virtual Transaction balanceOf(const Address& address, uint32_t nonce) = 0;

    virtual Transaction transfer(const Signer& from, const Address& to, uint64_t amount,
                                 const Signer& payer, uint64_t gasPrice, uint64_t gasLimit,
                                 uint32_t nonce) = 0;

    virtual Transaction unsignedTransfer(const Address& from, const Address& to, uint64_t amount,
                                         const Address& payer, uint64_t gasPrice, uint64_t gasLimit, 
                                         uint32_t nonce) = 0;
};
} // namespace TW::Ontology
