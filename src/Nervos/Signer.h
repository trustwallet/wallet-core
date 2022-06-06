// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"

namespace TW::Nervos {

class Signer {
  public:
    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& signingInputProto) noexcept {
        Proto::SigningOutput output;
        Common::Proto::SigningError error;
        auto signingInput = SigningInput(signingInputProto);
        Transaction tx;

        // Plan transaction
        error = tx.plan(signingInput);
        if (error != Common::Proto::OK) {
            // planning failed
            output.set_error(error);
            return output;
        }

        // Sign transaction
        error = tx.sign(signingInput.privateKeys);
        if (error != Common::Proto::OK) {
            // signing failed
            output.set_error(error);
            return output;
        }

        *output.mutable_transaction() = tx.proto();

        auto txHash = tx.hash();
        output.set_transaction_id(hex(txHash));

        return output;
    }
};

} // namespace TW::Nervos
