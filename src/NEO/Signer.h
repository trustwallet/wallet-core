// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PrivateKey.h"
#include "../Data.h"
#include "Address.h"
#include "Transaction.h"

namespace TW::NEO {

class Signer {
  private:
    Data publicKey;
    TW::PrivateKey privateKey;
    std::string address;
  public:
    explicit Signer(const TW::PrivateKey &priKey);
    PrivateKey getPrivateKey() const;
    PublicKey getPublicKey() const;
    Address getAddress() const;

    void sign(Transaction &tx) const;
    Data sign(const Data &data) const;

    /// Signs a Proto::SigningInput transaction
    //static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction.
    //static Data sign(const PrivateKey& privateKey, Transaction& transaction) noexcept;
};

} // namespace TW::NEO

/// Wrapper for C interface.
struct TWNEOSigner {
    TW::NEO::Signer impl;
};
