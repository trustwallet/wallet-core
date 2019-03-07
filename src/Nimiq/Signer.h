// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "Transaction.h"

namespace TW {
namespace Nimiq {

/// Helper class that performs Nimiq transaction signing.
class Signer {
public:
    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept;

    template<typename T>
    static PublicKey publicKeyFromBytes(const T& data) {
        assert(data.size() == 32);
        std::vector<uint8_t> pubkeyInternal;
        pubkeyInternal.push_back(0x01); // Code for Ed25519
        pubkeyInternal.insert(pubkeyInternal.end(), data.begin(), data.end());
        return PublicKey(std::move(pubkeyInternal));
    }
};

}} // namespace

/// Wrapper for C interface.
struct TWNimiqSigner {
    TW::Nimiq::Signer impl;
};
