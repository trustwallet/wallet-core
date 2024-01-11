// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "OutPoint.h"
#include "Script.h"
#include "Amount.h"
#include "../proto/Bitcoin.pb.h"

#include <vector>

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

    UTXO(const Proto::UnspentTransaction& utxo)
        : outPoint(utxo.out_point())
        , script(utxo.script().begin(), utxo.script().end())
        , amount(utxo.amount())
        {}

    Proto::UnspentTransaction proto() const {
        auto utxo = Proto::UnspentTransaction();
        *utxo.mutable_out_point() = outPoint.proto();
        utxo.set_script(std::string(script.bytes.begin(), script.bytes.end()));
        utxo.set_amount(amount);
        return utxo;
    }
};

/// A list of UTXO's
class UTXOs: public std::vector<UTXO> {
public:
    UTXOs() = default;
    UTXOs(const std::vector<UTXO>& vector): std::vector<UTXO>(vector) {}
    UTXOs(UTXO utxo): std::vector<UTXO>({utxo}) {}
};

} // namespace TW::Bitcoin
