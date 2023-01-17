// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <PrivateKey.h>
#include <string>
#include <optional>

namespace TW::Ethereum {

enum class MessageType {
    ImmutableX = 0,
    Legacy = 1,
    Eip155 = 2
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
    static std::string signMessage(const PrivateKey& privateKey, const std::string& message, MessageType msgType = MessageType::Legacy, MaybeChainId chainId = std::nullopt);
    /// Verify a message following EIP-191
    /// \param publicKey publickey to verify the signed message
    /// \param message message to be verified as a string
    /// \param signature signature to verify the message against
    /// \return true if the message match the signature, false otherwise
    static bool verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept;
    static constexpr auto MessagePrefix = "Ethereum Signed Message:\n";
    static constexpr std::uint8_t EthereumPrefix{0x19};
};

} // namespace TW::Ethereum
