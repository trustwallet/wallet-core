// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Wallet.h"
#include "Messages.h"

#include "HexCoding.h"

namespace TW::Everscale {

// WalletV3 contract https://github.com/tonlabs/ton-1/blob/master/crypto/smartcont/wallet3-code.fc
const Data Wallet::code = parse_hex("b5ee9c720101010100710000deff0020dd2082014c97ba218201339cbab19f71b0ed44d0d31fd31f31d70bffe304e0a4f2608308d71820d31fd31fd31ff82313bbf263ed44d0d31fd31fd3ffd15132baf2a15144baf2a204f901541055f910f2a3f8009320d74a96d307d402fb00e8d101a4c8cb1fcb1fcbffc9ed54");

CellBuilder InitData::writeTo() const {
    CellBuilder builder;

    builder.appendU32(_seqno);
    builder.appendU32(_walletId);
    builder.appendRaw(_publicKey.bytes, 256);

    return builder;
}

AddressData InitData::computeAddr(int8_t workchainId) const {
    auto builder = this->writeTo();

    StateInit stateInit{
        .code = Cell::deserialize(Wallet::code.data(), Wallet::code.size()),
        .data = builder.intoCell(),
    };
    return AddressData(workchainId, stateInit.writeTo().intoCell()->hash);
}

StateInit InitData::makeStateInit() const {
    auto builder = this->writeTo();

    return StateInit{
        .code = Cell::deserialize(Wallet::code.data(), Wallet::code.size()),
        .data = builder.intoCell(),
    };
}

CellBuilder InitData::makeTransferPayload(uint32_t expireAt, const Wallet::Gift& gift) const {
    CellBuilder payload;

    // insert prefix
    payload.appendU32(_walletId);
    payload.appendU32(expireAt);
    payload.appendU32(_seqno);

    // create internal message
    MessageData::HeaderRef header = std::make_shared<InternalMessageHeader>(true, gift.bounce, gift.to.addressData, gift.amount);
    auto message = Message(MessageData(header));

    // append it to the body
    payload.appendU8(gift.flags);
    payload.appendReferenceCell(message.intoCell());

    return payload;
}

} // namespace TW::Everscale
