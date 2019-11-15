// Copyright © 2017-2019 Trust Wallet.
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

class Signer {
public:
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
