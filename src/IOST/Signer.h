// Copyright © 2017-2019 Trust.
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
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    static std::string buildUnsignedTx(const Proto::SigningInput& input, const Data& pubkey,
                                       uint8_t algorithm) noexcept;
    static Proto::SigningOutput buildSignedTx(const Proto::SigningInput& input, const Data& pubkey,
                                              uint8_t algorithm, const Data& signature) noexcept;
};
} // namespace TW::IOST
