// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <vector>

namespace TW::Avalanche {

class TransactionInput{
  public: 
    virtual void encode (Data& data) const;
  protected:
    TransactionInput(){}  
};

/// Avalanche transaction input.
class TransferableInput {
  public:
    Data TxID;
    uint32_t UTXOIndex;
    Data AssetID;
    TransactionInput Input;

    /// Encodes the input into the provided buffer.
    void encode(Data& data) const;

    TransferableInput(Data &txid, uint32_t utxoIndex, Data &assetID, TransactionInput &input)
        : TxID(txid) , UTXOIndex(utxoIndex)
        , AssetID(assetID), Input(input) {}

    
    bool operator<(const TransferableInput& other) const;
};


class SECP256k1TransferInput : public TransactionInput {
  const uint32_t typeID = 5;
  
  public:
    uint64_t Amount;
    std::vector<uint32_t> AddressIndices;

    SECP256k1TransferInput(uint64_t amount, std::vector<uint32_t> addressIndices)
      : Amount(amount), AddressIndices(addressIndices) {
        std::sort(AddressIndices.begin(), AddressIndices.end());
      }
  
    void encode (Data& data) const;
};

} // namespace TW::Avalanche
