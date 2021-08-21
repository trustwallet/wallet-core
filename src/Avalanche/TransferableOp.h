// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Output.h"
#include "TransferableOutput.h"
#include "../Data.h"
#include <vector>

namespace TW::Avalanche {

// clang-format off

enum OperationTypeID {
    SECPMintOp = 8,
    NFTMintOp = 12,
    NFTTransferOp = 13
};

// clang-format on

class TransactionOp {
  public:
    /// Encodes the op into the provided buffer.
    virtual void encode(Data& data) const = 0; // we want to enforce that all subclasses can encode
    virtual ~TransactionOp() {}
    virtual std::unique_ptr<TransactionOp> duplicate() = 0;

  protected:
    TransactionOp() {}
};

/// Avalanche transaction operation.
class TransferableOp {
  public:
    using TxID = Data;
    using OutputIndex = uint32_t;
    using UTXOID = std::pair<TxID, OutputIndex>;
    std::vector<UTXOID> utxoIDs;

  private:
    static bool sortUTXOIDs(UTXOID lhs, UTXOID rhs);

  public:
    Data assetID;
    std::unique_ptr<TransactionOp> transferOp;

    /// Encodes the op into the provided buffer.
    void encode(Data& data) const;

    TransferableOp(Data& assetID, std::vector<UTXOID>& utxoIDs,
                   std::unique_ptr<TransactionOp> transferOp)
        : utxoIDs(utxoIDs), assetID(assetID), transferOp(std::move(transferOp)) {
        std::sort(this->utxoIDs.begin(), this->utxoIDs.end(), sortUTXOIDs);
    }

    TransferableOp(const TransferableOp& other) {
        utxoIDs = other.utxoIDs;
        std::sort(utxoIDs.begin(), utxoIDs.end(), sortUTXOIDs);
        assetID = other.assetID;
        transferOp = other.transferOp->duplicate();
    }

    bool operator<(const TransferableOp& other) const;

    TransferableOp& operator=(const TransferableOp& other);
};

class SECP256k1MintOperation : public TransactionOp {
  private:
    uint32_t typeID = OperationTypeID::SECPMintOp;

  public:
    std::vector<uint32_t> addressIndices;
    SECP256k1MintOutput mintOutput;
    SECP256k1TransferOutput transferOutput;

    SECP256k1MintOperation(std::vector<uint32_t>& addressIndices, SECP256k1MintOutput& mintOutput,
                           SECP256k1TransferOutput& transferOutput)
        : addressIndices(addressIndices), mintOutput(mintOutput), transferOutput(transferOutput) {
        std::sort(this->addressIndices.begin(), this->addressIndices.end());
    }

    void encode(Data& data) const;

    std::unique_ptr<TransactionOp> duplicate() {
        auto dup =
            std::make_unique<SECP256k1MintOperation>(addressIndices, mintOutput, transferOutput);
        return dup;
    }
};

class NFTMintOperation : public TransactionOp {
  private:
    uint32_t typeID = OperationTypeID::NFTMintOp;
    std::vector<Output> outputs;

  public:
    std::vector<uint32_t> addressIndices;
    uint32_t groupID;
    Data payload;

    NFTMintOperation(std::vector<uint32_t>& addressIndices, uint32_t groupID, Data& payload,
                     std::vector<Output>& outputs)
        : outputs(outputs), addressIndices(addressIndices), groupID(groupID), payload(payload) {
        std::sort(this->addressIndices.begin(), this->addressIndices.end());
        SortOutputs(this->outputs);
    }

    void encode(Data& data) const;

    std::unique_ptr<TransactionOp> duplicate() {
        auto dup = std::make_unique<NFTMintOperation>(addressIndices, groupID, payload, outputs);
        return dup;
    }
};

class NFTTransferOperation : public TransactionOp {
  private:
    uint32_t typeID = OperationTypeID::NFTTransferOp;

  public:
    std::vector<uint32_t> addressIndices;
    NFTTransferOutput transferOutput;

    NFTTransferOperation(std::vector<uint32_t>& addressIndices, NFTTransferOutput& transferOutput)
        : addressIndices(addressIndices), transferOutput(transferOutput) {
        std::sort(this->addressIndices.begin(), this->addressIndices.end());
        std::sort(this->transferOutput.addresses.begin(), this->transferOutput.addresses.end());
    }

    void encode(Data& data) const;

    std::unique_ptr<TransactionOp> duplicate() {
        auto dup = std::make_unique<NFTTransferOperation>(addressIndices, transferOutput);
        return dup;
    }
};

} // namespace TW::Avalanche
