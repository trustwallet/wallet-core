// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PrivateKey.h"
#include "../proto/IOST.pb.h"

namespace TW::IOST {
class Signer {
  public:
    Proto::SigningInput input;

    /// Initializes a transaction signer.
    explicit Signer(const Proto::SigningInput& input) : input(input) {}

    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    static std::string encodeTransaction(const Proto::Transaction& t) noexcept;

    Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey) const;
    Data signaturePreimage() const;
};
} // namespace TW::IOST
