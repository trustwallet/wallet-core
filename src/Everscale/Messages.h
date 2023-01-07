// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "CommonTON/Messages.h"

namespace TW::Everscale {

using MessageData = CommonTON::MessageData;
using InternalMessageHeader = CommonTON::InternalMessageHeader;

class Message {
private:
    MessageData _messageData;
public:
    explicit Message(const MessageData& messageData) : _messageData(messageData) {}
    [[nodiscard]] Cell::Ref intoCell() const;
    inline void setBody(const Cell::Ref& body) { _messageData.body = body; }
    inline void setStateInit(const StateInit& stateInit) { _messageData.init = stateInit; }
};

static auto createSignedMessage = CommonTON::createSignedMessage;

} // namespace TW::Everscale
