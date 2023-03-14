// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
