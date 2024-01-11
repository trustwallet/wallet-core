// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Nimiq.pb.h"

namespace TW::Nimiq {

/// Helper class that performs Nimiq transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
  public:
    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept;

    template <typename T>
    static PublicKey publicKeyFromBytes(const T& data) {
        assert(data.size() == 32);
        std::vector<uint8_t> pubkeyInternal;
        pubkeyInternal.push_back(0x01); // Code for Ed25519
        pubkeyInternal.insert(pubkeyInternal.end(), data.begin(), data.end());
        return PublicKey(std::move(pubkeyInternal), TWPublicKeyTypeED25519);
    }
};

} // namespace TW::Nimiq
