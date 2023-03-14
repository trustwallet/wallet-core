// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
