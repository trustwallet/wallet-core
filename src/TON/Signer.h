// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"
#include "../PublicKey.h"
#include "../PrivateKey.h"

#include <string>

namespace TW::TON {

/// Helper class that performs TON transaction signing.
class Signer {
public:
    /// Hide default constructor
    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    //static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction.
    //static Data sign(const PrivateKey& privateKey, Transaction& transaction) noexcept;

    /// sign a message
    static TW::Data sign(const PrivateKey& privateKey, const Data& message) noexcept;
    /// Build the external message for account initialization
    static TW::Data buildInitMessage(const PrivateKey& privkey);

protected:
    /// Build the external message for account initialization
    static TW::Data buildInitMessage(
        byte chainId, const PublicKey& pubkey, const TW::Data& signature, const TW::Data& msg
    );
};

} // namespace TW::TON

/// Wrapper for C interface.
struct TWTONSigner {
    TW::TON::Signer impl;
};
