// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "Transaction.h"
#include "../PrivateKey.h"

#include <array>
#include <string>
#include <vector>


namespace TW::Ontology {

class Signer {
  private:
    Data publicKey;
    TW::PrivateKey privateKey;
    std::string address;

  public:
    explicit Signer(TW::PrivateKey priKey);

    PrivateKey getPrivateKey() const;

    PublicKey getPublicKey() const;

    Address getAddress() const;

    void sign(Transaction& tx) const;

    void addSign(Transaction& tx) const;
};
} // namespace TW::Ontology

/// Wrapper for C interface.
struct TWOntologySigner {
    TW::Ontology::Signer impl;
};
