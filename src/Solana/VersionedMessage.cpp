// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Solana/VersionedMessage.h"
#include "Solana/Constants.h"
#include "Solana/Encoding.h"

namespace TW::Solana {

Data serialize(const VersionnedMessage& message) {
    auto visit_functor = [](const VersionnedMessage& message) -> Data {
        if (auto* msg = std::get_if<V0Message>(&message); msg) {
            Data out;
            append(out, MESSAGE_VERSION_PREFIX);
            append(out, msg->msg.serialize());
            append(out, shortVecLength<AddressLookupTable>(msg->addressTableLookups));
            return out;
        } else if (auto* legacyMsg = std::get_if<LegacyMessage>(&message); msg) {
            return legacyMsg->serialize();
        } else {
            return {};
        }
    };

    return std::visit(visit_functor, message);
}

} // namespace TW::Solana
