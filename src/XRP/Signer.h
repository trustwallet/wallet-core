// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"

namespace TW::Ripple {

/// Helper class that performs Ripple transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept;

  private:
    static void signPayment(const Proto::SigningInput& input,
                     Proto::SigningOutput& output,
                     Transaction& transaction) noexcept;

    static void signNfTokenCancelOffer(const Proto::SigningInput& input, Transaction& transaction) noexcept;
};

} // namespace TW::Ripple
