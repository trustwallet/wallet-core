// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../proto/Cosmos.pb.h"

#include <TrustWalletCore/TWCoinType.h>

namespace TW::Cosmos {

/// Helper class that performs Cosmos transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input, TWCoinType coin) noexcept;

    /// Signs a Proto::SigningInput transaction, using Json serialization
    static Proto::SigningOutput signJsonSerialized(const Proto::SigningInput& input) noexcept;

    /// Signs a Proto::SigningInput transaction, using binary Protobuf serialization
    static Proto::SigningOutput signProtobuf(const Proto::SigningInput& input, TWCoinType coin) noexcept;

    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key, TWCoinType coin);
};

} // namespace TW::Cosmos
