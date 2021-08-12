// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "Script.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "TransactionPlan.h"
#include "UTXO.h"
#include "../PrivateKey.h"
#include "../Hash.h"
#include "../Data.h"
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

    // List of transaction inputs
    TransactionInputs<TransactionInput> inputs;

    // List of transaction outputs
    TransactionOutputs<TransactionOutput> outputs;

    TW::Hash::Hasher hasher = TW::Hash::sha256d;

    /// Used for diagnostics; store previously estimated virtual size (if any; size in bytes)
    int previousEstimatedVirtualSize = 0;

public:
    Transaction() = default;

    Transaction(int32_t version, uint32_t lockTime, TW::Hash::Hasher hasher = TW::Hash::sha256d)
        : version(version), lockTime(lockTime), inputs(), outputs(), hasher(hasher) {}

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

// TODO move to separate file
class SigningInput {
public:
    // Hash type to use when signing
    TWBitcoinSigHashType hashType;

    // Amount to send.  Transaction created will have this amount in its output, 
    // except when use_max_amount is set, in that case this amount is not relevant, maximum possible amount will be used (max avail less fee).
    // If amount is equal or more than the available amount, also max amount will be used.
    Amount amount;

    // Transaction fee per byte
    Amount byteFee;

    // Recipient's address
    std::string toAddress;

    // Change address
    std::string changeAddress;

    // Available private keys
    std::vector<PrivateKey> privateKeys;

    // Available redeem scripts indexed by script hash
    std::map<std::string, Script> scripts;

    // Available unspent transaction outputs
    std::vector<UTXO> utxos;

    // If sending max amount
    bool useMaxAmount;

    // Coin type (forks)
    TWCoinType coinType;

    // Optional transaction plan
    std::optional<TransactionPlan> plan;

public:
    SigningInput() = default;

    SigningInput(const Proto::SigningInput& input);
};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWBitcoinTransaction {
    TW::Bitcoin::Transaction impl;
};
