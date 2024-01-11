// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
