// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Bitcoin/Script.h"
#include "TransactionInput.h"
#include "../Bitcoin/TransactionOutput.h"
#include "../Hash.h"
//XXXX #include "../proto/Iocoin.pb.h"
#include <TrustWalletCore/TWBitcoin.h>
#include <vector>

namespace TW::Iocoin {

struct Transaction {
    /// Transaction data format version (note, this is signed)
    int32_t version = 1;

    /// The block number or timestamp at which this transaction is unlocked
    ///
    ///     | Value          | Description
    ///     |----------------|------------
    ///     |  0             | Not locked
    ///     | < 500000000    | Block number at which this transaction is unlocked
    ///     | >= 500000000   | UNIX timestamp at which this transaction is unlocked
    ///
    /// If all inputs have final (`0xffffffff`) sequence numbers then `lockTime` is irrelevant. Otherwise, the
    /// transaction may not be added to a block until after `lockTime`.
    uint32_t lockTime = 0;

    uint32_t nTime = 0;
    /// A list of 1 or more transaction inputs or sources for coins
    std::vector<TransactionInput> inputs;

    /// A list of 1 or more transaction outputs or destinations for coins
    std::vector<Bitcoin::TransactionOutput> outputs;

    TW::Hash::Hasher hasher = TW::Hash::sha256d;

    Transaction() = default;

    Transaction(const Transaction& other)
      : version(other.version), lockTime(other.lockTime), nTime(other.nTime), inputs(other.inputs), outputs(other.outputs)
     {
     }

    /// Whether the transaction is empty.
    bool empty() const { return inputs.empty() && outputs.empty(); }

    /// Generates the signature pre-image.
    //std::vector<uint8_t> getPreImage(const Bitcoin::Script& scriptCode, size_t index, uint32_t hashType,
    //                                 uint64_t amount) const;
    std::vector<uint8_t> getPrevoutHash() const;
    std::vector<uint8_t> getSequenceHash() const;
    std::vector<uint8_t> getOutputsHash() const;

    /// Encodes the transaction into the provided buffer.
    void encode(std::vector<uint8_t>& data) const;

    /// Generates the signature hash for this transaction.
    std::vector<uint8_t> getSignatureHash(const Bitcoin::Script& scriptCode, size_t index, uint32_t hashType
                                          ) const;

    void serializeInput(size_t subindex, const Bitcoin::Script&, size_t index, uint32_t hashType,
                        std::vector<uint8_t>& data) const;

    /// Converts to Protobuf model
    Proto::Transaction proto() const; 

};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWIocoinTransaction {
    TW::Iocoin::Transaction impl;
};
