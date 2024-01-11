// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Data.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../proto/FIO.pb.h"

#include <string>

namespace TW::FIO {

/// FIO Signer, signing primitives.  See also TransactionBuilder for building full signed transaction
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    /// Build the compile output
    static Proto::SigningOutput compile(const Proto::SigningInput& input, const Data& signature) noexcept;
  public:
    static constexpr auto SignatureSuffix = "K1";
    static constexpr auto SignaturePrefix = "SIG_K1_";

    /// Sign the hash of the provided data
    static Data signData(const PrivateKey& privKey, const Data& data);

    /// Used internally, encode signature to base58 with prefix. Ex.: "SIG_K1_K54CA1jmhgWrSdvrNrkokPyvqh7dwsSoQHNU9xgD3Ezf6cJySzhKeUubVRqmpYdnjoP1DM6SorroVAgrCu3qqvJ9coAQ6u"
    static std::string signatureToBase58(const Data& sig);

    /// Verify a signature, used in testing
    static bool verify(const PublicKey& pubKey, const Data& data, const Data& signature);

    static int isCanonical(uint8_t by, uint8_t sig[64]);
};

} // namespace TW::FIO
