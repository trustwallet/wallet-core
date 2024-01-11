// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <PrivateKey.h>
#include <string>
#include <optional>

namespace TW::Tron {

class MessageSigner {
public:
    /// Sign a message following https://github.com/tronprotocol/tronweb/blob/859253856c79d3aff26ec6c89afefc73840d648d/src/lib/trx.js#L768
    /// \param privateKey the private key to sign with
    /// \param message message to sign
    /// \return hex signed message
    static std::string signMessage(const PrivateKey& privateKey, const std::string& message);

    /// Verify a TRON message
    /// \param publicKey publickey to verify the signed message
    /// \param message message to be verified as a string
    /// \param signature signature to verify the message against
    /// \return true if the message match the signature, false otherwise
    static bool verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept;
    static constexpr auto MessagePrefix = "TRON Signed Message:\n32";
    static constexpr std::uint8_t TronPrefix{0x19};
};

} // namespace TW::Ethereum
