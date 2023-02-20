// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <variant>
#include "Solana/V0Message.h"
#include "Solana/LegacyMessage.h"
#include "Solana/MessageHeader.h"

namespace TW::Solana {
    using VersionedMessage = std::variant<V0Message, LegacyMessage>;

    Data serialize(const VersionedMessage& message);
    MessageHeader header(const VersionedMessage& message);
    std::vector<Address> accountKeys(const VersionedMessage& message);
    void updateRecentHash(VersionedMessage& message, const Data& recentHash);
}
