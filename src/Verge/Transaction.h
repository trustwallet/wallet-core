// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "../Bitcoin/Transaction.h"
#include "../PrivateKey.h"
#include "../Hash.h"
#include "../Data.h"
#include "../proto/Bitcoin.pb.h"

#include "../Bitcoin/Script.h"
#include "../Bitcoin/SignatureVersion.h"

#include <vector>

namespace TW::Verge {

struct Transaction : public Bitcoin::Transaction {
public:
    /// Transaction time
    uint32_t time = 0;

public:
    Transaction() = default;

    Transaction(int32_t version, uint32_t time = 0, uint32_t lockTime = 0, TW::Hash::Hasher hasher = TW::Hash::HasherSha256d)
        : Bitcoin::Transaction(version, lockTime, hasher)
        , time(time) {}

    /// Generates the signature pre-image.
    Data getPreImage(const Bitcoin::Script& scriptCode, size_t index, enum TWBitcoinSigHashType hashType, uint64_t amount) const;

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data, enum SegwitFormatMode segwitFormat) const;

    /// Default one-parameter version, needed for templated usage.
    void encode(Data& data) const { encode(data, SegwitFormatMode::IfHasWitness); }

    /// Generates the signature hash for this transaction.
    Data getSignatureHash(const Bitcoin::Script& scriptCode, size_t index, enum TWBitcoinSigHashType hashType,
                          uint64_t amount, enum Bitcoin::SignatureVersion version) const;

private:
    /// Generates the signature hash for Witness version 0 scripts.
    Data getSignatureHashWitnessV0(const Bitcoin::Script& scriptCode, size_t index,
                                   enum TWBitcoinSigHashType hashType, uint64_t amount) const;

    /// Generates the signature hash for for scripts other than witness scripts.
    Data getSignatureHashBase(const Bitcoin::Script& scriptCode, size_t index,
                              enum TWBitcoinSigHashType hashType) const;
};

} // namespace TW::Verge
