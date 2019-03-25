// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>
#include <vector>
#include <string>

#include "Data.h"
#include "Signer.h"
#include "Address.h"
#include "Transaction.h"
#include "BinaryCoding.h"

namespace TW {
namespace Ontology {

class Asset {
protected:

    const uint8_t txType = 0xD1;

public:
    virtual Data contractAddress() = 0;

    virtual  Transaction decimals(const Address &address) = 0;

    virtual Transaction balanceOf(const Address &address) = 0;

    virtual Transaction transfer(const Signer &from, const Address &to, uint64_t amount, const Signer &payer, uint64_t gasPrice, uint64_t gasLimit) = 0;

};
}} // namespace
