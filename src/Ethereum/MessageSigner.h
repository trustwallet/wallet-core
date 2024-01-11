// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <PrivateKey.h>
#include <string>
#include <optional>

namespace TW::Ethereum {

enum class MessageType {
    Legacy = 1,
    Eip155 = 2,
    ImmutableX = 3
};

class MessageSigner {
public:
    using MaybeChainId = std::optional<std::size_t>;
    /// Sign a message following EIP-191
    /// \param privateKey the private key to sign with
    /// \param message message to sign
    /// \param msgType message type to sign
    /// \param chainId optional chainId if msgType is eip155
    /// \return hex signed message
    static std::string signMessage(const PrivateKey& privateKey, const std::string& message, MessageType msgType, MaybeChainId chainId = std::nullopt);

    /// Sign typed data according to EIP-712 V4
    /// \param privateKey the private key to sign with
    /// \param data json data
    /// \param msgType message type to sign
    /// \param chainId optional chainId if msgType is eip155
    /// \return hex signed message
    static std::string signTypedData(const PrivateKey& privateKey, const std::string& data, MessageType msgType, MaybeChainId chainId = std::nullopt);

    /// Verify a message following EIP-191
    /// \param publicKey publickey to verify the signed message
    /// \param message message to be verified as a string
    /// \param signature signature to verify the message against
    /// \return true if the message match the signature, false otherwise
    static bool verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept;

    /// Computes a hash of the message following EIP-191.
    /// \param message message to hash
    /// \return hash of the tuped data.
    static Data messagePreImageHash(const std::string& message) noexcept;

    /// Computes a hash of the typed data according to EIP-712 V4.
    /// \param data json data
    /// \return hash of the tuped data.
    static Data typedDataPreImageHash(const std::string& data) noexcept;

    static void prepareSignature(Data& signature, MessageType msgType, MaybeChainId chainId = std::nullopt) noexcept;
};

} // namespace TW::Ethereum
