// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "Address.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Stellar.pb.h"

#include <string>

namespace TW::Stellar {
/// Helper class that performs Ripple transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
  public:
    const Proto::SigningInput& input;

    Signer(const Proto::SigningInput& input) : input(input) {}

    /// Signs the given transaction.
    std::string sign() const noexcept;

    Data encode(const Proto::SigningInput& input) const;

  private:
    static uint32_t operationType(const Proto::SigningInput& input);
    static void encodeAddress(const Address& address, Data& data);
    static void encodeAsset(const Proto::Asset& asset, Data& data);
    void pad(Data& data) const;
};

} // namespace TW::Stellar
