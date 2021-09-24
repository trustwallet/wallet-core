// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Data.h"
#include "../PrivateKey.h"
#include "../proto/Avalanche.pb.h"

namespace TW::Avalanche {

/// Helper wrapper for proto input class
class InputProtoWrapper {
public:
    Proto::TransferableInput _proto;
    InputProtoWrapper(const Proto::TransferableInput& input): _proto(input) {}
    uint64_t amount() const {
        if (_proto.input().has_secp_transfer_input()) {
            return _proto.input().secp_transfer_input().amount();
        }
        return 0;
    }
};

/// Helper class that performs Avalanche transaction signing.
class Signer {
  public:
    /// Hide default constructor
    Signer() = delete;

    /// Returns a transaction plan (utxo selection, fee estimation)
    static Proto::TransactionPlan plan(const Proto::SigningInput& input) noexcept;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction.
    static Data sign(const std::vector<PrivateKey>& privateKeys,
                     BaseTransaction& transaction) noexcept;
};

} // namespace TW::Avalanche
