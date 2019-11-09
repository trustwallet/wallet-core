// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PrivateKey.h"
#include "../proto/Zilliqa.pb.h"

namespace TW::Zilliqa {

/// Helper class that performs Zilliqa transaction signing.
class Signer {
  public:
    Signer() = delete;

    /// compute preImage from signing input.
    static Data getPreImage(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction preImage.
    static Proto::SigningOutput sign(const Data& preImage, const PrivateKey& key) noexcept;
};

} // namespace TW::Zilliqa

/// Wrapper for C interface.
struct TWZilliqaSigner {
    TW::Zilliqa::Signer impl;
};
