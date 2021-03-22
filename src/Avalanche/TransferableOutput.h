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

class TransactionOutput {
  public:
    /// Encodes the output into the provided buffer.
    virtual void encode(Data& data) const = 0;

    bool operator<(const TransactionOutput& other) const;

  protected:
    TransactionOutput(){}
    virtual ~TransactionOutput(){}
};

/// Avalanche transaction output.
class TransferableOutput {
  public:
    Data AssetID;
    TransactionOutput* Output;

    /// Encodes the output into the provided buffer.
    void encode(Data& data) const;

    TransferableOutput(Data &assetID, TransactionOutput &output)
      : AssetID(assetID), Output(&output) {}
    
    bool operator<(const TransferableOutput& other) const;
      
};


class SECP256k1TransferOutput : public TransactionOutput {
  const uint32_t typeID = 7;
  
  public:
    uint64_t Amount;
    uint64_t Locktime;
    uint32_t Threshold;
    std::vector<Address> Addresses;

    SECP256k1TransferOutput(uint64_t amount, uint64_t locktime, uint32_t threshold, std::vector<Address> &addresses)
      : Amount(amount), 
      Locktime(locktime), Threshold(threshold), Addresses(addresses) {
        std::sort(Addresses.begin(), Addresses.end());
      }
  
    void encode (Data& data) const;
};


class SECP256k1MintOutput : public TransactionOutput {
  const uint32_t typeID = 6;
  
  public:
    uint64_t Locktime;
    uint32_t Threshold;
    std::vector<Address> Addresses;

    SECP256k1MintOutput(uint64_t locktime, uint32_t threshold, std::vector<Address> &addresses)
      : Locktime(locktime), Threshold(threshold), Addresses(addresses) {
        std::sort(Addresses.begin(), Addresses.end());
      }
  
    void encode (Data& data) const;
};

class NFTTransferOutput : public TransactionOutput {
  const uint32_t typeID = 11;
  
  public:
    uint32_t GroupID;
    Data Payload;
    uint64_t Locktime;
    uint32_t Threshold;
    std::vector<Address> Addresses;

    NFTTransferOutput(uint32_t groupID, Data &payload, uint64_t locktime, uint32_t threshold, std::vector<Address> &addresses)
      : GroupID(groupID), Payload(payload),
      Locktime(locktime), Threshold(threshold), Addresses(addresses) {
        std::sort(Addresses.begin(), Addresses.end());
      }
  
    void encode (Data& data) const;
};

class NFTMintOutput : public TransactionOutput {
  const uint32_t typeID = 10;
  
  public:
    uint32_t GroupID;
    uint64_t Locktime;
    uint32_t Threshold;
    std::vector<Address> Addresses;

    NFTMintOutput(uint32_t groupID,uint64_t locktime, uint32_t threshold, std::vector<Address> &addresses)
      : GroupID(groupID),
      Locktime(locktime), Threshold(threshold), Addresses(addresses) {
        std::sort(Addresses.begin(), Addresses.end());
      }
  
    void encode (Data& data) const;
};

} // namespace TW::Avalanche
