// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "Data.h"
#include <vector>

namespace TW::Avalanche {

enum TransactionOutputTypeID {
    SECPTransferOut = 7,
    SECPMint = 6,
    NFTTransferOut = 11,
    NFTMintOut = 10
};

class TransactionOutput {
  public:
    /// Encodes the output into the provided buffer.
    virtual void encode(Data& data) const = 0;
    bool operator<(const TransactionOutput& other) const;
    virtual ~TransactionOutput() {}
    virtual std::unique_ptr<TransactionOutput> duplicate() = 0;

  protected:
    TransactionOutput() {}
};

/// Avalanche transaction output.
class TransferableOutput {
  public:
    Data assetID;
    std::unique_ptr<TransactionOutput> output;

    /// Encodes the output into the provided buffer.
    void encode(Data& data) const;

    TransferableOutput(const Data& assetID, std::unique_ptr<TransactionOutput> output)
        : assetID(assetID), output(std::move(output)) {}

    TransferableOutput(const TransferableOutput& other) {
        assetID = other.assetID;
        output = other.output->duplicate();
    }

    TransferableOutput& operator=(const TransferableOutput& other);

    bool operator<(const TransferableOutput& other) const;
};

class SECP256k1TransferOutput : public TransactionOutput {
    const uint32_t typeID = TransactionOutputTypeID::SECPTransferOut;

  public:
    uint64_t amount;
    uint64_t locktime;
    uint32_t threshold;
    std::vector<Address> addresses;

    SECP256k1TransferOutput(uint64_t amount, uint64_t locktime, uint32_t threshold,
                            std::vector<Address>& addresses)
        : amount(amount), locktime(locktime), threshold(threshold), addresses(addresses) {
        std::sort(this->addresses.begin(), this->addresses.end());
    }

    void encode(Data& data) const;

    std::unique_ptr<TransactionOutput> duplicate() {
        auto dup =
            std::make_unique<SECP256k1TransferOutput>(amount, locktime, threshold, addresses);
        return dup;
    }
};

class SECP256k1MintOutput : public TransactionOutput {
    const uint32_t typeID = TransactionOutputTypeID::SECPMint;

  public:
    uint64_t locktime;
    uint32_t threshold;
    std::vector<Address> addresses;

    SECP256k1MintOutput(uint64_t locktime, uint32_t threshold, std::vector<Address>& addresses)
        : locktime(locktime), threshold(threshold), addresses(addresses) {
        std::sort(this->addresses.begin(), this->addresses.end());
    }

    void encode(Data& data) const;

    std::unique_ptr<TransactionOutput> duplicate() {
        auto dup = std::make_unique<SECP256k1MintOutput>(locktime, threshold, addresses);
        return dup;
    }
};

class NFTTransferOutput : public TransactionOutput {
    const uint32_t typeID = TransactionOutputTypeID::NFTTransferOut;

  public:
    uint32_t groupID;
    Data payload;
    uint64_t locktime;
    uint32_t threshold;
    std::vector<Address> addresses;

    NFTTransferOutput(uint32_t groupID, Data& payload, uint64_t locktime, uint32_t threshold,
                      std::vector<Address>& addresses)
        : groupID(groupID)
        , payload(payload)
        , locktime(locktime)
        , threshold(threshold)
        , addresses(addresses) {
        std::sort(this->addresses.begin(), this->addresses.end());
    }

    void encode(Data& data) const;

    std::unique_ptr<TransactionOutput> duplicate() {
        auto dup =
            std::make_unique<NFTTransferOutput>(groupID, payload, locktime, threshold, addresses);
        return dup;
    }
};

class NFTMintOutput : public TransactionOutput {
    const uint32_t typeID = TransactionOutputTypeID::NFTMintOut;

  public:
    uint32_t groupID;
    uint64_t locktime;
    uint32_t threshold;
    std::vector<Address> addresses;

    NFTMintOutput(uint32_t groupID, uint64_t locktime, uint32_t threshold,
                  std::vector<Address>& addresses)
        : groupID(groupID), locktime(locktime), threshold(threshold), addresses(addresses) {
        std::sort(this->addresses.begin(), this->addresses.end());
    }

    void encode(Data& data) const;

    std::unique_ptr<TransactionOutput> duplicate() {
        auto dup = std::make_unique<NFTMintOutput>(groupID, locktime, threshold, addresses);
        return dup;
    }
};

} // namespace TW::Avalanche
