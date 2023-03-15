// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Everscale/CommonTON/Messages.h"

using namespace TW::CommonTON;

namespace TW::TheOpenNetwork {

class Message {
private:
    MessageData _messageData;

public:
    explicit Message(MessageData messageData) : _messageData(std::move(messageData)) {}

    [[nodiscard]] Cell::Ref intoCell() const;

    inline void setBody(const Cell::Ref& body) { _messageData.body = body; }
    inline void setStateInit(const StateInit& stateInit) { _messageData.init = stateInit; }
};

} // namespace TW::TheOpenNetwork
