// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "Address.h"
#include "../Data.h"
#include "../PrivateKey.h"
#include <proto/Vite.pb.h>

namespace TW::Vite {
/// Helper class that performs Ripple transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
  public:
    const PrivateKey privateKey;
    const Proto::SigningInput& input;
    const Data blockHash;

    explicit Signer(const Proto::SigningInput& input);

    /// Signs the blockHash, returns signature bytes
    Data sign() const noexcept;

    /// Builds signed transaction, incl. signature
    Proto::SigningOutput build() const;
};

} // namespace TW::Vite
