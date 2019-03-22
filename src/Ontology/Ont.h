// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <vector>

#include "Data.h"
#include "Asset.h"

namespace TW {
namespace Ontology {

class Ont : public Asset {
private:

    static const uint8_t version = 0x00;
    
    const Data ontContract = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

public:

    Data contractAddress() override {
        return ontContract;
    }

    Transaction balanceOf(const Address &address) override;

    Transaction transfer(const Account &from, const Address &to, uint64_t amount, const Account &payer, uint64_t gasPrice, uint64_t gasLimit) override;

};


}} // namespace
