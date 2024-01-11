// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Transaction.h"
#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/NEO.pb.h"

namespace TW::NEO {

class Signer {
private:
    Data publicKey;
    TW::PrivateKey privateKey;
    Address address;

public:
    explicit Signer(const TW::PrivateKey& priKey);
    PrivateKey getPrivateKey() const;
    PublicKey getPublicKey() const;
    Address getAddress() const;

    static Proto::TransactionPlan plan(const Proto::SigningInput& input);
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    void sign(Transaction& tx) const;
    Data sign(const Data& data) const;

    static Data signaturePreimage(const Proto::SigningInput& input);
    static Data encodeTransaction(const Proto::SigningInput& input,
                                  const std::vector<PublicKey>& publicKeys,
                                  const std::vector<Data>& signatures);

private:
    static std::shared_ptr<Transaction>
    prepareUnsignedTransaction(const Proto::SigningInput& input, const Proto::TransactionPlan& plan,
                               bool validate = true);
};

} // namespace TW::NEO
