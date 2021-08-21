// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"
#include <vector>

namespace TW::Avalanche {

enum TransactionInputTypeID { SECPInput = 5 };

class TransactionInput {
  public:
    virtual std::vector<uint32_t> getAddressIndices() const = 0;
    virtual uint32_t getTypeID() const = 0;
    virtual void encode(Data& data) const = 0; // we want to enforce that all subclasses can encode
    virtual ~TransactionInput() {}
    virtual std::unique_ptr<TransactionInput> duplicate() = 0;

  protected:
    TransactionInput() {}
};

/// Avalanche transaction input.
class TransferableInput {
  public:
    Data txID;
    uint32_t utxoIndex;
    Data assetID;
    std::unique_ptr<TransactionInput> input;
    std::vector<Address>
        spendableAddresses; // corresponding to the Output this came from. not encoded

    /// Encodes the input into the provided buffer.
    void encode(Data& data) const;

    TransferableInput(Data& txid, uint32_t utxoIndex, Data& assetID,
                      std::unique_ptr<TransactionInput> input,
                      std::vector<Address>& spendableAddresses)
        : txID(txid)
        , utxoIndex(utxoIndex)
        , assetID(assetID)
        , input(std::move(input))
        , spendableAddresses(spendableAddresses) {
        std::sort(this->spendableAddresses.begin(), this->spendableAddresses.end());
    }

    TransferableInput(const TransferableInput& other) {
        txID = other.txID;
        utxoIndex = other.utxoIndex;
        assetID = other.assetID;
        input = other.input->duplicate();
        spendableAddresses = other.spendableAddresses;
    }

    bool operator<(const TransferableInput& other) const;

    TransferableInput& operator=(const TransferableInput& other);
};

class SECP256k1TransferInput : public TransactionInput {
    uint32_t typeID = TransactionInputTypeID::SECPInput;
    uint64_t amount;
    std::vector<uint32_t> addressIndices;

  public:
    SECP256k1TransferInput(uint64_t amount, std::vector<uint32_t> addressIndices)
        : amount(amount), addressIndices(addressIndices) {
        std::sort(this->addressIndices.begin(), this->addressIndices.end());
    }

    void encode(Data& data) const;

    std::vector<uint32_t> getAddressIndices() const { return addressIndices; }
    uint32_t getTypeID() const { return typeID; }

    std::unique_ptr<TransactionInput> duplicate() {
        auto dup = std::make_unique<SECP256k1TransferInput>(amount, addressIndices);
        return dup;
    }
};

} // namespace TW::Avalanche
