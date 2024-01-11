// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include "../Bitcoin/Script.h"
#include "../Bitcoin/Transaction.h"
#include "../Bitcoin/TransactionInput.h"
#include "../Bitcoin/TransactionOutput.h"
#include "../proto/Bitcoin.pb.h"
#include "Data.h"

namespace TW::BitcoinDiamond {

struct Transaction : public Bitcoin::Transaction {
public:
    // see: https://github.com/eveybcd/BitcoinDiamond/blob/master/src/primitives/transaction.h#L209
    static const int32_t CURRENT_VERSION_FORK = 12;
    static const int32_t CURRENT_VERSION = CURRENT_VERSION_FORK;

    Data preBlockHash;

public:
    Transaction() : Bitcoin::Transaction(CURRENT_VERSION, 0, TW::Hash::HasherSha256d) {}
    Transaction(const Data& blockHash, int32_t version = CURRENT_VERSION, uint32_t lockTime = 0)
        : Bitcoin::Transaction(version, lockTime, TW::Hash::HasherSha256d)
        , preBlockHash(blockHash) {}

    Data getPreImage(const Bitcoin::Script& scriptCode, size_t index, enum TWBitcoinSigHashType hashType, uint64_t amount) const;
    
    /// Encodes the transaction into the provided buffer.
    void encode(Data& data, enum SegwitFormatMode segwitFormat) const;
    void encode(Data& data) const { encode(data, SegwitFormatMode::IfHasWitness); }

    Data getSignatureHash(const Bitcoin::Script& scriptCode, size_t index, enum TWBitcoinSigHashType hashType,
                          uint64_t amount, enum Bitcoin::SignatureVersion version) const;
    /// Converts to Protobuf model
    Bitcoin::Proto::Transaction proto() const;

private:
    /// Generates the signature hash for Witness version 0 scripts.
    Data getSignatureHashWitnessV0(const Bitcoin::Script& scriptCode, size_t index,
                                   enum TWBitcoinSigHashType hashType, uint64_t amount) const;

    /// Generates the signature hash for for scripts other than witness scripts.
    Data getSignatureHashBase(const Bitcoin::Script& scriptCode, size_t index,
                              enum TWBitcoinSigHashType hashType) const;
};

} // namespace TW::BitcoinDiamond
