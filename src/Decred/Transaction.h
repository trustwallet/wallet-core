// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "../Bitcoin/Script.h"
#include "../Data.h"
#include "../proto/Decred.pb.h"

#include <TrustWalletCore/TWBitcoin.h>
#include <vector>

namespace TW {
namespace Decred {

enum class SerializeType : uint16_t { full, noWitness, onlyWitness };

struct Transaction {
    /// Serialization format
    SerializeType serializeType;

    /// Transaction data format version
    uint16_t version;

    /// A list of 1 or more transaction inputs or sources for coins
    std::vector<TransactionInput> inputs;

    /// A list of 1 or more transaction outputs or destinations for coins
    std::vector<TransactionOutput> outputs;

    /// The time when a transaction can be spent (usually zero, in which case it
    /// has no effect).
    uint32_t lockTime;

    /// The block height at which the transaction expires and is no longer
    /// valid.
    uint32_t expiry;

    Transaction()
        : serializeType(SerializeType::full)
        , version(1)
        , inputs()
        , outputs()
        , lockTime()
        , expiry(0) {}

    /// Whether the transaction is empty.
    bool empty() const { return inputs.empty() && outputs.empty(); }

    /// Generates the signature pre-image.
    Data computeSignatureHash(const Bitcoin::Script& scriptCode, int index,
                              uint32_t hashType) const;

    /// Generates the transaction hash.
    Data hash() const;

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    /// Converts to Protobuf model
    Proto::Transaction proto() const;

  private:
    Data computePrefixHash(const std::vector<TransactionInput>& inputsToSign,
                           const std::vector<TransactionOutput>& outputsToSign,
                           std::size_t signIndex, std::size_t index, uint32_t hashType) const;
    Data computeWitnessHash(const std::vector<TransactionInput>& inputsToSign,
                            const Bitcoin::Script& signScript, std::size_t signIndex) const;

    void encodePrefix(Data& data) const;
    void encodeWitness(Data& data) const;
};

} // namespace Decred
} // namespace TW
