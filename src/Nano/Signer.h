// Copyright © 2019 Mart Roosmaa.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "Address.h"
#include "../Data.h"
#include "../PrivateKey.h"
#include <proto/Nano.pb.h>

namespace TW::Nano {
/// Helper class that performs Ripple transaction signing.
class Signer {
  public:
    const PrivateKey privateKey;
    const PublicKey publicKey;
    const std::array<byte, 32> blockHash;

    explicit Signer(const Proto::SigningInput& input);

    /// Signs the blockHash
    std::array<byte, 64> sign() const noexcept;
};

} // namespace TW::Nano

/// Wrapper for C interface.
struct TWNanoSigner {
    TW::Nano::Signer impl;
};
