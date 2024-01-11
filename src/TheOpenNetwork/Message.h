// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
