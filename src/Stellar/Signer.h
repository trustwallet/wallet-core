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

    Data encode(Address address, uint32_t fee, uint64_t sequence, Address destination, uint64_t amount) const;

    void encodeAddress(Address address, Data& data) const;
};

}} // namespace

/// Wrapper for C interface.
struct TWStellarSigner {
    TW::Stellar::Signer impl;
};
