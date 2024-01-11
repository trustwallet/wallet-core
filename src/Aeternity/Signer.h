// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "../proto/Aeternity.pb.h"
#include <PrivateKey.h>

namespace TW::Aeternity {

class Signer {
public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction.
    static Proto::SigningOutput sign(const PrivateKey& privateKey, Transaction& transaction);

private:
    static const uint8_t checkSumSize = 4;

    static Data buildRlpTxRaw(const Data& txRaw, const Data& sigRaw);

    static Data buildMessageToSign(const Data& txRaw);

    static Proto::SigningOutput createProtoOutput(std::string& signature, const std::string& signedTx);

    /// Encode a byte array into base64 with prefix and a checksum
    static std::string encodeBase64WithChecksum(const std::string& prefix, const TW::Data& rawTx);
};

} // namespace TW::Aeternity
