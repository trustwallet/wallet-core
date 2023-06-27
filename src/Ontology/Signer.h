// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "Transaction.h"
#include "../PrivateKey.h"
#include "../proto/Ontology.pb.h"

#include <array>
#include <string>
#include <vector>

namespace TW::Ontology {

class Signer {
public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    static Data encodeTransaction(const Proto::SigningInput& input, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeyss);
private:
    Data publicKey; 
    TW::PrivateKey privKey;
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
