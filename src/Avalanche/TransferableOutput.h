// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <vector>

namespace TW::Avalanche {

class Output;

/// Avalanche transaction output.
class TransferableOutput {
  public:
    Data AssetID;
    Output Output;

    /// Encodes the input into the provided buffer.
    void encode(Data& data) const;

    // TransferableOutput(Data &txid, uint32_t utxoIndex, Data &assetID, SECP256k1TransferInput &input)
    //     : TxID(txid) , UTXOIndex(utxoIndex)
    //     , AssetID(assetID), Input(input) {}
};

class Output {
    /// Encodes the input into the provided buffer.
    void encode(Data& data) const;
};



} // namespace TW::Avalanche
