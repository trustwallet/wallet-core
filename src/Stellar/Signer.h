// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include <proto/Stellar.pb.h>
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "Address.h"

namespace TW {
namespace Stellar {
/// Helper class that performs Ripple transaction signing.
class Signer {
public:
    const Proto::SigningInput& input;

    Signer(const Proto::SigningInput& input) : input(input) {}

    /// Signs the given transaction.
    std::string sign() const noexcept;

    Data encode(const Proto::SigningInput& input) const;

private:
    void encodeAddress(const Address& address, Data& data) const;

    void pad(Data& data) const;
};

}} // namespace

/// Wrapper for C interface.
struct TWStellarSigner {
    TW::Stellar::Signer impl;
};
