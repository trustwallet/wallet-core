// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "Bitcoin/Transaction.h"
#include "Bitcoin/Script.h"
#include "Data.h"
#include "../proto/Decred.pb.h"

#include "Bitcoin/SignatureVersion.h"
#include <vector>

namespace TW::Decred {

enum class SerializeType : uint16_t { full, noWitness, onlyWitness };

struct Transaction {
    /// Serialization format
    SerializeType serializeType = SerializeType::full;

    /// Transaction data format version
    uint16_t version = 1;

    /// A list of 1 or more transaction inputs or sources for coins
    Bitcoin::TransactionInputs<TransactionInput> inputs;

    /// A list of 1 or more transaction outputs or destinations for coins
    Bitcoin::TransactionOutputs<TransactionOutput> outputs;

    /// The time when a transaction can be spent (usually zero, in which case it
    /// has no effect).
    uint32_t lockTime = 0;

    /// The block height at which the transaction expires and is no longer
    /// valid.
    uint32_t expiry = 0;

    Transaction()
        : inputs()
        , outputs() {}

    /// Whether the transaction is empty.
    bool empty() const { return inputs.empty() && outputs.empty(); }

    /// Generates the signature pre-image.
    Data computeSignatureHash(const Bitcoin::Script& scriptCode, size_t index,
                              enum TWBitcoinSigHashType hashType) const;

    /// Generates the transaction hash.
    Data hash() const;

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    /// Converts to Protobuf model
    Proto::Transaction proto() const;

  private:
    Data computePrefixHash(const std::vector<TransactionInput>& inputsToSign,
                           const std::vector<TransactionOutput>& outputsToSign,
                           std::size_t signIndex, std::size_t index, enum TWBitcoinSigHashType hashType) const;
    Data computeWitnessHash(const std::vector<TransactionInput>& inputsToSign,
                            const Bitcoin::Script& signScript, std::size_t signIndex) const;

    void encodePrefix(Data& data) const;
    void encodeWitness(Data& data) const;
};

} // namespace TW::Decred
