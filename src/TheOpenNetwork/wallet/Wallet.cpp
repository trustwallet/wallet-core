// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Wallet.h"

#include "HexCoding.h"

namespace TW::TheOpenNetwork {

Wallet::Wallet(PublicKey publicKey, int8_t workchainId, Data walletCode)
    : publicKey(publicKey)
    , workchainId(workchainId)
    , walletCode(walletCode)
    , walletId(698983191 + workchainId) {
}

Address Wallet::getAddress() {
    const Cell::Ref stateInit = this->createStateInit();
    auto address = Address(workchainId, stateInit->hash);

    address.isUserFriendly = true;
    address.isBounceable = true;
    address.isTestOnly = false;

    return address;
}

Cell::Ref Wallet::createStateInit() {
    const auto code = Cell::deserialize(walletCode.data(), walletCode.size());
    const auto data = this->createDataCell();

    CellBuilder builder;

    builder.appendBitZero();            // maybe<split_depth>
    builder.appendBitZero();            // maybe<special>

    builder.appendBitOne();             // maybe<code>
    builder.appendReferenceCell(code);  // code

    builder.appendBitOne();             // maybe<data>
    builder.appendReferenceCell(data);  // data

    builder.appendBitZero();            // library: empty hashmap

    return builder.intoCell();
}

} // namespace TW::TheOpenNetwork
