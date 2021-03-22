// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "Address.h"
#include <vector>

namespace TW::Avalanche {

class TransactionInput{
  public: 
    virtual std::vector<uint32_t> getAddressIndices() const = 0;
    virtual uint32_t getTypeID() const = 0;
    virtual void encode (Data& data) const = 0; //we want to enforce that all subclasses can encode
  protected:
    TransactionInput() {}
    virtual ~TransactionInput() {}
};

/// Avalanche transaction input.
class TransferableInput {
  public:
    Data TxID;
    uint32_t UTXOIndex;
    Data AssetID;
    TransactionInput* Input;
    std::vector<Address> SpendableAddresses; // corresponding to the Output this came from. not encoded

    /// Encodes the input into the provided buffer.
    void encode(Data& data) const;

    TransferableInput(Data &txid, uint32_t utxoIndex, Data &assetID, TransactionInput &input, std::vector<Address> &spendableAddresses)
        : TxID(txid) , UTXOIndex(utxoIndex), AssetID(assetID), Input(&input), SpendableAddresses(spendableAddresses)
         {
           std::sort(SpendableAddresses.begin(), SpendableAddresses.end());
         }

    
    bool operator<(const TransferableInput& other) const;
};


class SECP256k1TransferInput : public TransactionInput {
    uint32_t TypeID = 5; 
    uint64_t Amount;
    std::vector<uint32_t> AddressIndices;
  
  public:

    SECP256k1TransferInput(uint64_t amount, std::vector<uint32_t> addressIndices)
      : Amount(amount) {
        AddressIndices = addressIndices;
        std::sort(AddressIndices.begin(), AddressIndices.end());
      }
  
    void encode (Data& data) const;

    std::vector<uint32_t> getAddressIndices() const {return AddressIndices;}
    uint32_t getTypeID() const {return TypeID;}
};

} // namespace TW::Avalanche
