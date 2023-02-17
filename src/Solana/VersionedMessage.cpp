// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Solana/VersionedMessage.h"
#include "Solana/Constants.h"
#include "Solana/Encoding.h"

namespace TW::Solana {

Data serialize(const VersionedMessage& message) {
    auto visit_functor = [](const VersionedMessage& message) -> Data {
        if (auto* msg = std::get_if<V0Message>(&message); msg) {
            Data out;
            append(out, MESSAGE_VERSION_PREFIX);
            append(out, msg->msg.serialize());
            append(out, shortVecLength<AddressLookupTable>(msg->addressTableLookups));
            return out;
        } else if (auto* legacyMsg = std::get_if<LegacyMessage>(&message); legacyMsg) {
            return legacyMsg->serialize();
        } else {
            return {};
        }
    };

    return std::visit(visit_functor, message);
}

MessageHeader header(const VersionedMessage& message) {
    auto visit_functor = [](const VersionedMessage& message) -> MessageHeader {
        if (auto* msg = std::get_if<V0Message>(&message); msg) {
            return msg->msg.header;
        } else if (auto* legacyMsg = std::get_if<LegacyMessage>(&message); legacyMsg) {
            return legacyMsg->header;
        } else {
            return {};
        }
    };

    return std::visit(visit_functor, message);
}

std::vector<Address> accountKeys(const VersionedMessage& message) {
    auto visit_functor = [](const VersionedMessage& message) -> std::vector<Address> {
        if (auto* msg = std::get_if<V0Message>(&message); msg) {
            return msg->msg.accountKeys;
        } else if (auto* legacyMsg = std::get_if<LegacyMessage>(&message); legacyMsg) {
            return legacyMsg->accountKeys;
        } else {
            return {};
        }
    };

    return std::visit(visit_functor, message);
}

void updateRecentHash(VersionedMessage& message, const Data& recentHash) {
    if (auto* msg = std::get_if<V0Message>(&message); msg) {
        msg->msg.mRecentBlockHash = recentHash;
    } else if (auto* legacyMsg = std::get_if<LegacyMessage>(&message); legacyMsg) {
        legacyMsg->mRecentBlockHash = recentHash;
    }
}

} // namespace TW::Solana
