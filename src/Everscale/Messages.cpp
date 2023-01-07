// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Messages.h"
#include "WorkchainType.h"

using namespace TW;

namespace TW::Everscale {

Cell::Ref Message::intoCell() const {
    CellBuilder builder;

    // write Header
    _messageData.header->writeTo(builder);

    // write StateInit
    if (_messageData.init.has_value()) {
        auto initBuilder = _messageData.init.value().writeTo();

        builder.appendBitOne();
        builder.appendBitZero();
        builder.appendBuilder(initBuilder);
    } else {
        builder.appendBitZero();
    }

    // write Body
    if (_messageData.body.has_value()) {
        builder.appendBitZero();
        builder.appendCellSlice(CellSlice(_messageData.body.value().get()));
    } else {
        builder.appendBitZero();
    }

    return builder.intoCell();
}

} // namespace TW::Everscale
