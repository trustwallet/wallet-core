// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "TransferableOutput.h"
#include "Output.h"
#include <vector>

namespace TW::Avalanche {

class TransactionOp {
  public:
    /// Encodes the op into the provided buffer.
    virtual void encode(Data& data) const = 0;  //we want to enforce that all subclasses can encode
    virtual ~TransactionOp(){}
    virtual TransactionOp* duplicate() = 0;
  protected:
    TransactionOp(){}
};

/// Avalanche transaction operation.
class TransferableOp {
  public: 
    using TxID = Data;
    using OutputIndex = uint32_t;
    using UTXOID = std::pair<TxID, OutputIndex>;
    std::vector<UTXOID> UTXOIDs;
  private:
    static bool sortUTXOIDs(UTXOID lhs, UTXOID rhs);
  public:
    Data AssetID;
    TransactionOp* TransferOp;

    /// Encodes the op into the provided buffer.
    void encode(Data& data) const;

    TransferableOp(Data &assetID, std::vector<UTXOID> &utxoIDs, TransactionOp *transferOp)
      : AssetID(assetID), UTXOIDs(utxoIDs), TransferOp(transferOp) {
        std::sort(UTXOIDs.begin(), UTXOIDs.end(), sortUTXOIDs);
      }

    TransferableOp(const TransferableOp& other) {
      UTXOIDs = other.UTXOIDs;
      std::sort(UTXOIDs.begin(), UTXOIDs.end(), sortUTXOIDs);
      AssetID = other.AssetID;
      TransferOp = other.TransferOp->duplicate();
    }

    bool operator<(const TransferableOp& other) const;

    TransferableOp& operator=(const TransferableOp& other);
    
    ~TransferableOp();
};


class SECP256k1MintOperation : public TransactionOp {
  private:
    uint32_t typeID = 8;
  public: 
    std::vector<uint32_t> AddressIndices;
    SECP256k1MintOutput MintOutput;
    SECP256k1TransferOutput TransferOutput;

    SECP256k1MintOperation(std::vector<uint32_t> &addressIndices, SECP256k1MintOutput &mintOutput, SECP256k1TransferOutput &transferOutput)
      : AddressIndices(addressIndices), MintOutput(mintOutput), TransferOutput(transferOutput) {
        std::sort(AddressIndices.begin(), AddressIndices.end());
      }

    void encode (Data& data) const;

    TransactionOp* duplicate() {
      auto dup = new SECP256k1MintOperation(AddressIndices, MintOutput, TransferOutput);
      return dup;
    }
};

class NFTMintOperation : public TransactionOp {
  private:
    uint32_t typeID = 12;
    std::vector<Output> Outputs; 
  public: 
    std::vector<uint32_t> AddressIndices;
    uint32_t GroupID;
    Data Payload;

    NFTMintOperation(std::vector<uint32_t> &addressIndices, uint32_t groupID, Data &payload, std::vector<Output> &outputs)
    : AddressIndices(addressIndices), GroupID(groupID), Payload(payload), Outputs(outputs) {
      std::sort(AddressIndices.begin(), AddressIndices.end());
      SortOutputs(Outputs);
    }

    void encode (Data& data) const;

    TransactionOp* duplicate() {
      auto dup = new NFTMintOperation(AddressIndices, GroupID, Payload, Outputs);
      return dup;
    }
};

class NFTTransferOperation : public TransactionOp {
  private:
    uint32_t typeID = 13;
  public: 
    std::vector<uint32_t> AddressIndices;
    NFTTransferOutput TransferOutput;

    NFTTransferOperation(std::vector<uint32_t> &addressIndices, NFTTransferOutput &transferOutput)
      : AddressIndices(addressIndices), TransferOutput(transferOutput) {
        std::sort(AddressIndices.begin(), AddressIndices.end());
        std::sort(TransferOutput.Addresses.begin(), TransferOutput.Addresses.end());
      }

    void encode (Data& data) const;

    TransactionOp* duplicate() {
      auto dup = new NFTTransferOperation(AddressIndices, TransferOutput);
      return dup;
    }
};

} // namespace TW::Avalanche
