// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/Script.h"
#include "../Bitcoin/Transaction.h"
#include "../Bitcoin/TransactionInput.h"
#include "../Bitcoin/TransactionOutput.h"
#include "../proto/Bitcoin.pb.h"

#include <vector>

namespace TW {
namespace Zcash {

/// Only supports Sapling transparent transaction right now
/// See also https://github.com/zcash/zips/blob/master/zip-0243.rst
struct Transaction {
    uint32_t version;
    uint32_t versionGroupId;
    uint32_t lockTime;
    uint32_t expiryHeight;
    uint64_t valueBalance;

    std::vector<Bitcoin::TransactionInput> inputs;
    std::vector<Bitcoin::TransactionOutput> outputs;

    Transaction()
        : version(0x80000004)
        , versionGroupId(0x892F2085)
        , lockTime()
        , expiryHeight()
        , valueBalance()
        {}

    Transaction(uint32_t version, uint32_t versionGroupId, uint32_t lockTime, uint32_t expiryHeight, uint64_t valueBalance)
        : version(version)
        , versionGroupId(versionGroupId)
        , lockTime(lockTime)
        , expiryHeight(expiryHeight)
        , valueBalance(valueBalance)
        {}

    /// Whether the transaction is empty.
    bool empty() const {
        return inputs.empty() && outputs.empty();
    }

    /// Generates the signature pre-image.
    Data getPreImage(const Bitcoin::Script& scriptCode, int index, uint32_t hashType, uint64_t amount) const;
    Data getPrevoutHash() const;
    Data getSequenceHash() const;
    Data getOutputsHash() const;

    Data getJoinSplitsHash() const;
    Data getShieldedSpendsHash() const;
    Data getShieldedOutputsHash() const;

    /// Encodes the rawtx into the provided buffer.
    void encode(Data& data) const;

    Data getSignatureHash(const Bitcoin::Script& scriptCode, size_t index, uint32_t hashType, uint64_t amount, TWBitcoinSignatureVersion version) const;

    /// Converts to Protobuf model
    Bitcoin::Proto::Transaction proto() const;
};

}} // namespace

/// Wrapper for C interface.
struct TWZcashTransaction {
    TW::Zcash::Transaction impl;
};
