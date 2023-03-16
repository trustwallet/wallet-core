// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PrivateKey.h"

#include <string>

namespace TW::Bitcoin {

/// Class for message signing and verification.
///
/// Bitcoin Core and some other wallets support a message signing & verification format, to create a proof (a signature)
/// that someone has access to the private keys of a specific address.
/// This feature currently works on old legacy addresses only.
class MessageSigner {
  public:
    /// Sign a message.
    /// privateKey: the private key used for signing
    /// address: the address that matches the privateKey, must be a legacy address (P2PKH)
    /// message: A custom message which is input to the signing.
    /// compressed: True by default, as addresses are generated from the hash of the compressed public key.
    ///    However, in some instances key hash is generated from the hash of the extended public key,
    ///    that's also supported here as well for compatibility.
    /// Returns the signature, Base64-encoded.
    /// Throws on invalid input.
    static std::string signMessage(const PrivateKey& privateKey, const std::string& address, const std::string& message, bool compressed = true);
  
    /// Verify signature for a message.
    /// address: address to use, only legacy is supported
    /// message: the message signed (without prefix)
    /// signature: in Base64-encoded form.
    /// Returns false on any invalid input (does not throw).
    static bool verifyMessage(const std::string& address, const std::string& message, const std::string& signature) noexcept;

    /// Verify signature for a message.
    /// Address: address to use, only legacy is supported
    /// message: the message signed (without prefix)
    /// signature: in binary form.
    /// May throw
    static bool verifyMessage(const std::string& address, const std::string& message, const Data& signature);

    /// Recover address from signature and message. May throw.
    static std::string recoverAddressFromMessage(const std::string& message, const Data& signature);

    /// Append prefix and compute hash for a message
    static Data messageToHash(const std::string& message);

    static constexpr auto MessagePrefix = "Bitcoin Signed Message:\n";
    static const byte DigestLength = 32;
    static const byte SignatureRSLength = 64;
    static constexpr byte SignatureRSVLength = SignatureRSLength + 1;
    static const byte VOffset = 27;
};

} // namespace TW::Bitcoin
