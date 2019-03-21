// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>
#include <vector>
#include <string>

#include "Account.h"
#include "Transaction.h"
#include "BinaryCoding.h"

namespace TW {
namespace Ontology {

class Asset {

protected:

    const uint8_t txType = 0xD1;

public:
    virtual std::vector<uint8_t> contractAddress() = 0;

    virtual Transaction balanceOf(const std::string &address) = 0;

    virtual Transaction transfer(const Account &from, std::string &to, uint64_t amount, const Account &payer, uint64_t gasPrice, uint64_t gasLimit) = 0;

};
}} // namespace
