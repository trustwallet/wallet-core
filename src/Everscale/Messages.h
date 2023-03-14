// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "CommonTON/Messages.h"

using namespace TW::CommonTON;

namespace TW::Everscale {

class Message {
private:
    MessageData _messageData;

public:
    explicit Message(MessageData messageData) : _messageData(std::move(messageData)) {}
    [[nodiscard]] Cell::Ref intoCell() const;
};

MessageData createSignedMessage(PublicKey& publicKey, PrivateKey& key, bool bounce, uint32_t flags, uint64_t amount,
                                uint32_t expiredAt, Address destination, const Cell::Ref& contractData);

} // namespace TW::Everscale
