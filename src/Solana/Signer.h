// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Solana.pb.h"

namespace TW::Solana {

/// Helper class that performs Solana transaction signing.
class Signer {
  public:
    /// Signs the given transaction.
    static void sign(const std::vector<PrivateKey>& privateKeys, Transaction& transaction);
    static void signUpdateBlockhash(const std::vector<PrivateKey>& privateKeys,
                                    Transaction& transaction, Solana::Hash& recentBlockhash);
    static Data signRawMessage(const std::vector<PrivateKey>& privateKeys, const Data messageData);

    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
};

} // namespace TW::Solana
