// Copyright © 2017-2022 Trust Wallet.
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
class MessageSigner {
  public:
    // TODO desc
    static Data signMessage(const PrivateKey& privateKey, const std::string& address, const std::string& message, bool compressed = true);
  
    /// Verify signature for a message.
    /// Address: address to use, only legacy is supported
    /// message: the message signed (without prefix)
    /// signature: in Base64-encoded form.
    static bool verifyMessage(const std::string& address, const std::string& message, const std::string& signature);

    /// Verify signature for a message.
    /// Address: address to use, only legacy is supported
    /// message: the message signed (without prefix)
    /// signature: in binary form.
    /// May throw
    static bool verifyMessage(const std::string& address, const std::string& message, const Data& signature);

    static constexpr const char* MessagePrefix = "Bitcoin Signed Message:\n";
};

} // namespace TW::Bitcoin
