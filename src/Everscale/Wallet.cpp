#include "CellBuilder.h"
#include "Messages.h"
#include "Wallet.h"

using namespace TW;
using namespace TW::Everscale;

Cell::Ref InitData::intoCell() const {
    CellBuilder dataBuilder;

    dataBuilder.appendU32(seqno_);
    dataBuilder.appendU32(walletId_);
    dataBuilder.appendRaw(publicKey_.bytes, 256);

    return dataBuilder.intoCell();
}

Address::MsgAddressInt InitData::computeAddr(int8_t workchainId) const {
    const auto data = this->intoCell();
    const auto code = Cell::deserialize(Wallet::code, sizeof(Wallet::code));

    StateInit stateInit(code, data);
    return std::make_pair(workchainId, stateInit.intoCell()->hash);
}

StateInit InitData::makeStateInit() const {
    const auto data = this->intoCell();
    const auto code = Cell::deserialize(Wallet::code, sizeof(Wallet::code));

    return StateInit(code, data);
}

std::pair<Cell::CellHash, CellBuilder> InitData::makeTransferPayload(uint32_t expireAt, const Wallet::Gift& gift) const {
    CellBuilder payload;

    // insert prefix
    payload.appendU32(walletId_);
    payload.appendU32(expireAt);
    payload.appendU32(seqno_);

    // create internal message
    Message::HeaderRef header = std::make_shared<InternalMessageHeader>(true, gift.bounce, gift.destination, gift.amount);
    auto message = Message(header);

    // append it to the body
    payload.appendU8(gift.flags);
    payload.appendReferenceCell(message.intoCell());

    auto hash = payload.intoCell()->hash;
    return std::make_pair(hash, payload);
}

Cell::Ref StateInit::intoCell() const {
    CellBuilder stateInitBuilder;

    stateInitBuilder.appendBitZero(); // split_depth
    stateInitBuilder.appendBitZero(); // special
    stateInitBuilder.appendBitOne();  // code
    stateInitBuilder.appendReferenceCell(code_);
    stateInitBuilder.appendBitOne(); // data
    stateInitBuilder.appendReferenceCell(data_);
    stateInitBuilder.appendBitZero(); // library

    return stateInitBuilder.intoCell();
}
