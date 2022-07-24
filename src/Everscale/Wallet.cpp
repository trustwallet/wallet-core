#include "CellBuilder.h"
#include "Wallet.h"

using namespace TW;
using namespace TW::Everscale;

std::array<byte, Address::size> InitData::computeAddr() const {
    const auto data = InitData::intoCell();
    const auto code = Cell::deserialize(Wallet::code, sizeof(Wallet::code));

    StateInit stateInit(code, data);
    return stateInit.intoCell()->hash;
}

Cell::Ref InitData::intoCell() const {
    CellBuilder dataBuilder;
    dataBuilder.appendU32(seqno);
    dataBuilder.appendU32(walletId);
    dataBuilder.appendRaw(publicKey.bytes, 256);

    return dataBuilder.intoCell();
}

Cell::Ref StateInit::intoCell() const {
    CellBuilder stateInitBuilder;

    stateInitBuilder.appendBitZero(); // split_depth
    stateInitBuilder.appendBitZero(); // special
    stateInitBuilder.appendBitOne();  // code
    stateInitBuilder.appendReferenceCell(code);
    stateInitBuilder.appendBitOne(); // data
    stateInitBuilder.appendReferenceCell(data);
    stateInitBuilder.appendBitZero(); // library

    return stateInitBuilder.intoCell();
}
