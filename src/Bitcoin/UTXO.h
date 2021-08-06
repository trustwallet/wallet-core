// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"
#include "Script.h"
#include "Amount.h"

namespace TW::Bitcoin {

class UTXO {
public:
    // The unspent output
    OutPoint outPoint;

    // Script for claiming this UTXO
    Script script;

    // Amount of the UTXO
    Amount amount;

public:
    UTXO() = default;

    // TODO move to .cpp
    UTXO(const Proto::UnspentTransaction& utxo)
        : outPoint(utxo.out_point())
        , script(utxo.script().begin(), utxo.script().end())
        , amount(utxo.amount())
        {}

    // TODO move to .cpp
    Proto::UnspentTransaction proto() const {
        auto utxo = Proto::UnspentTransaction();
        *utxo.mutable_out_point() = outPoint.proto();
        utxo.set_script(std::string(script.bytes.begin(), script.bytes.end()));
        utxo.set_amount(amount);
        return utxo;
    }
};

} // namespace TW::Bitcoin
