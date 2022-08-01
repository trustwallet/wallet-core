#include "CellBuilder.h"
#include "Messages.h"
#include "Wallet.h"

using namespace TW;

namespace TW::Everscale {

CellBuilder InitData::writeTo() const {
    CellBuilder builder;

    builder.appendU32(_seqno);
    builder.appendU32(_walletId);
    builder.appendRaw(_publicKey.bytes, 256);

    return builder;
}

Address::MsgAddressInt InitData::computeAddr(int8_t workchainId) const {
    auto builder = this->writeTo();
    const auto data = builder.intoCell();
    const auto code = Cell::deserialize(Wallet::code, sizeof(Wallet::code));

    StateInit stateInit(code, data);
    return std::make_pair(workchainId, stateInit.writeTo().intoCell()->hash);
}

StateInit InitData::makeStateInit() const {
    auto builder = this->writeTo();

    const auto data = builder.intoCell();
    const auto code = Cell::deserialize(Wallet::code, sizeof(Wallet::code));

    return StateInit(code, data);
}

CellBuilder InitData::makeTransferPayload(uint32_t expireAt, const Wallet::Gift& gift) const {
    CellBuilder payload;

    // insert prefix
    payload.appendU32(_walletId);
    payload.appendU32(expireAt);
    payload.appendU32(_seqno);

    // create internal message
    Message::HeaderRef header = std::make_shared<InternalMessageHeader>(true, gift.bounce, gift.destination, gift.amount);
    auto message = Message(header);

    // append it to the body
    payload.appendU8(gift.flags);
    payload.appendReferenceCell(message.intoCell());

    return payload;
}

CellBuilder StateInit::writeTo() const {
    CellBuilder builder;

    builder.appendBitZero(); // split_depth
    builder.appendBitZero(); // special
    builder.appendBitOne();  // code
    builder.appendReferenceCell(_code);
    builder.appendBitOne(); // data
    builder.appendReferenceCell(_data);
    builder.appendBitZero(); // library

    return builder;
}

}
