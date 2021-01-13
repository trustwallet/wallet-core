// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <vector>

namespace TW::Avalanche {

class TransferOp;

/// Avalanche transaction operation.
class TransferableOp {
  public:
    Data AssetID;
    using TxID = Data;
    using OutputIndex = uint32_t;
    using UTXOID = std::pair<TxID, OutputIndex>;
    std::vector<UTXOID> UTXOIDs;
    TransferOp TransferOp;

    /// Encodes the op into the provided buffer.
    void encode(Data& data) const;

    // TransferableOutput(Data &txid, uint32_t utxoIndex, Data &assetID, SECP256k1TransferInput &input)
    //     : TxID(txid) , UTXOIndex(utxoIndex)
    //     , AssetID(assetID), Input(input) {}
};

class TransferOp {
    /// Encodes the op into the provided buffer.
    void encode(Data& data) const;
};



} // namespace TW::Avalanche
