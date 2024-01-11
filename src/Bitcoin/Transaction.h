// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "Script.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "TransactionPlan.h"
#include "UTXO.h"
#include "../PrivateKey.h"
#include "../Hash.h"
#include "Data.h"
#include "SignatureVersion.h"
#include "../proto/Bitcoin.pb.h"

#include <vector>
#include <optional>

namespace TW::Bitcoin {

/// A list of transaction inputs
template <typename TransactionInput>
class TransactionInputs: public std::vector<TransactionInput> {};

/// A list of transaction outputs
template <typename TransactionOutput>
class TransactionOutputs: public std::vector<TransactionOutput> {};

struct Transaction {
public:
    /// Transaction data format version (note, this is signed)
    int32_t _version = 1;

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

    // List of transaction inputs
    TransactionInputs<TransactionInput> inputs;

    // List of transaction outputs
    TransactionOutputs<TransactionOutput> outputs;

    TW::Hash::Hasher hasher = TW::Hash::HasherSha256d;

    /// Used for diagnostics; store previously estimated virtual size (if any; size in bytes)
    int previousEstimatedVirtualSize = 0;

public:
    Transaction() = default;

    Transaction(int32_t version, uint32_t lockTime = 0, TW::Hash::Hasher hasher = TW::Hash::HasherSha256d)
        : _version(version), lockTime(lockTime), inputs(), outputs(), hasher(hasher) {}

    /// Whether the transaction is empty.
    bool empty() const { return inputs.empty() && outputs.empty(); }

    /// Generates the signature pre-image.
    Data getPreImage(const Script& scriptCode, size_t index, enum TWBitcoinSigHashType hashType, uint64_t amount) const;
    Data getPrevoutHash() const;
    Data getSequenceHash() const;
    Data getOutputsHash() const;

    enum SegwitFormatMode {
        NonSegwit,
        IfHasWitness,
        Segwit
    };

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data, enum SegwitFormatMode segwitFormat) const;

    /// Default one-parameter version, needed for templated usage.
    void encode(Data& data) const { encode(data, SegwitFormatMode::IfHasWitness); }

    /// Encodes the witness part of the transaction into the provided buffer.
    void encodeWitness(Data& data) const;

    bool hasWitness() const;

    /// Generates the signature hash for this transaction.
    Data getSignatureHash(const Script& scriptCode, size_t index, enum TWBitcoinSigHashType hashType,
                          uint64_t amount, enum SignatureVersion version) const;

    void serializeInput(size_t subindex, const Script&, size_t index, enum TWBitcoinSigHashType hashType, Data& data) const;

    static std::optional<uint64_t> calculateFee(const Data& encoded, uint64_t satVb);

    /// Converts to Protobuf model
    Proto::Transaction proto() const;

private:
    /// Generates the signature hash for Witness version 0 scripts.
    Data getSignatureHashWitnessV0(const Script& scriptCode, size_t index,
                                   enum TWBitcoinSigHashType hashType, uint64_t amount) const;

    /// Generates the signature hash for for scripts other than witness scripts.
    Data getSignatureHashBase(const Script& scriptCode, size_t index,
                              enum TWBitcoinSigHashType hashType) const;
};

} // namespace TW::Bitcoin
