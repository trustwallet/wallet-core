// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <PrivateKey.h>
#include <string>

namespace TW::StarkEx {
class MessageSigner {
public:
    /// Sign a message following StarkEx Curve
    /// \param privateKey the private key to sign with
    /// \param message hex message to sign
    /// \return hex signed message
    static std::string signMessage(const PrivateKey& privateKey, const std::string& message);;

    /// Verify a message following EIP-191
    /// \param publicKey publickey to verify the signed message
    /// \param message message to be verified as a string
    /// \param signature signature to verify the message against
    /// \return true if the message match the signature, false otherwise
    static bool verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept;;
};
}
