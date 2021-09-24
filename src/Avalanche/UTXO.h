// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TransferableOutput.h"
#include "../Data.h"

namespace TW::Avalanche {

class UTXO {
  public:
    const uint16_t codecID = 0;
    Data txID;
    uint32_t utxoIndex;
    Data assetID;
    TransferableOutput output;

    UTXO(const Data& txID, uint32_t utxoIndex, const Data& assetID,
         const TransferableOutput& output)
        : txID(txID), utxoIndex(utxoIndex), assetID(assetID), output(output) {}

    /// Encodes the UTXO into the provided buffer.
    void encode(Data& data) const;
};

} // namespace TW::Avalanche
