// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "../PublicKey.h"
#include "../PrivateKey.h"
#include "../Data.h"

namespace TW::Bitshares {
class Memo {
public:
    PublicKey from, to;
    uint64_t nonce;
    Data encryptedMessage;

    // Nonce should not be set manually except for debugging
    Memo(const PrivateKey& senderKey, const PublicKey& recipientKey, const std::string& message, uint64_t nonce = 0);

    void serialize(Data& os) const noexcept;
    nlohmann::json serialize() const noexcept;

    static Data getSharedSecret(const PrivateKey& senderKey, const PublicKey& recipientKey);
};
} // namespace TW::Bitshares