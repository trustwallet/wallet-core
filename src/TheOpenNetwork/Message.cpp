// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Message.h"

namespace TW::TheOpenNetwork {

Cell::Ref Message::intoCell() const {
    CellBuilder builder;

    // info:CommonMsgInfo
    _messageData.header->writeTo(builder);

    // init:(Maybe (Either StateInit ^StateInit))
    if (_messageData.init.has_value()) {
        builder.appendBitOne(); // Maybe

        builder.appendBitOne(); // Either as ^X
        builder.appendReferenceCell(_messageData.init.value().writeTo().intoCell());
    } else {
        builder.appendBitZero();
    }

    // body:(Either X ^X)
    if (_messageData.body.has_value()) {
        builder.appendBitOne(); // Either as ^X
        builder.appendReferenceCell(_messageData.body.value());
    } else {
        builder.appendBitZero();
    }

    return builder.intoCell();
}

} // namespace TW::TheOpenNetwork
