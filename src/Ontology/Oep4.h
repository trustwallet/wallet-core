// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Asset.h"
#include "Data.h"
#include "ParamsBuilder.h"
#include "Transaction.h"

namespace TW::Ontology {

class Oep4 {
private:
    static constexpr uint8_t version = 0x00;

    Data oep4Contract;
    // building neovm instruction for oep4 readonly method(name, symbol...)
    // are all the same except the method name
    Transaction readOnlyMethod(std::string methodName, uint32_t nonce);

public:
    explicit Oep4(const Address addr) noexcept;
    explicit Oep4(const Data bin) noexcept;
    Oep4() = delete;
    Data contractAddress() { return oep4Contract; }
    Transaction name(uint32_t nonce);
    Transaction symbol(uint32_t nonce);
    Transaction decimals(uint32_t nonce);
    Transaction totalSupply(uint32_t nonce);
    Transaction balanceOf(const Address& address, uint32_t nonce);
    Transaction transfer(const Signer& from, const Address& to, uint64_t amount,
                         const Signer& payer, uint64_t gasPrice, uint64_t gasLimit,
                         uint32_t nonce);
    Transaction unsignedTransfer(const Address& from, const Address& to, uint64_t amount,
                                 const Address& payer, uint64_t gasPrice, uint64_t gasLimit, uint32_t nonce);
};
} // namespace TW::Ontology
