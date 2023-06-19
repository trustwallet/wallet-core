// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../proto/Icon.pb.h"

#include <map>
#include <string>

namespace TW::Icon {

class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
  public:
    const Proto::SigningInput& input;

    Signer(const Proto::SigningInput& input) : input(input) {}

    /// Signs an Icon transaction.
    Proto::SigningOutput sign() const noexcept;

    /// Signature pre-image.
    std::string preImage() const noexcept;

    /// Encodes a signed transaction as JSON.
    std::string encode(const Data& signature) const noexcept;

    TW::Data hashImage(const Data& image) const;

  private:
    std::map<std::string, std::string> parameters() const noexcept;
};

} // namespace TW::Icon
