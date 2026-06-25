// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
#pragma once

#include "Address.h"
#include "Data.h"
#include "../Result.h"
#include "../proto/Stellar.pb.h"

#include <string>

namespace TW::Stellar {
/// Helper class that performs Ripple transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input);

  public:
    const Proto::SigningInput& _input;

    Signer(const Proto::SigningInput& input) : _input(input) {}

    /// Signs the given transaction.
    Result<std::string> sign() const;

    Result<Data> encode(const Proto::SigningInput& input) const;
    Result<Data> signaturePreimage() const;
    Proto::SigningOutput compile(const Data& sig) const;

  private:
    static uint32_t operationType(const Proto::SigningInput& input);
    static void encodeAddress(const Address& address, Data& data);
    static void encodeAsset(const Proto::Asset& asset, Data& data);
    void pad(Data& data) const;
};

} // namespace TW::Stellar
