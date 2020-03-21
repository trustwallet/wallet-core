// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PrivateKey.h"
#include "../proto/BitcoinGold.pb.h"

#include "../Bitcoin/Signer.h"

namespace TW::BitcoinGold {

/// Helper class that performs BitcoinGold transaction signing.
class Signer : public TW::Bitcoin::Signer {
public:
    /// Hide default constructor
    Signer() = delete;

    /// BitcoinGold transaction use Bitcoin signer
    // static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
};

} // namespace TW::BitcoinGold

/// Wrapper for C interface.
struct TWBitcoinGoldSigner {
    TW::BitcoinGold::Signer impl;
};
