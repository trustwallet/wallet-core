// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BinaryCoding.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include "Everscale/Cell.h"
#include "Everscale/CellBuilder.h"
#include "Everscale/Wallet.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Everscale {

TEST(EverscaleCell, ComputeContractAddress) {
    const auto seqno = 0;
    const auto walletId = WALLET_ID;
    const auto publicKey = PublicKey(parse_hex("7dbe83e9b223157e85bed2628430e2cdb531d5c99ab428618b7dd29b567a0369"), TWPublicKeyTypeED25519);

    CellBuilder dataBuilder;
    dataBuilder.appendU32(seqno);
    dataBuilder.appendU32(walletId);
    dataBuilder.appendRaw(publicKey.bytes, 256);

    const auto data = dataBuilder.intoCell();
    const auto code = Cell::deserialize(Wallet::code.data(), Wallet::code.size());

    CellBuilder stateInitBuilder;
    stateInitBuilder.appendBitZero(); // split_depth
    stateInitBuilder.appendBitZero(); // special
    stateInitBuilder.appendBitOne();  // code
    stateInitBuilder.appendReferenceCell(code);
    stateInitBuilder.appendBitOne(); // data
    stateInitBuilder.appendReferenceCell(data);
    stateInitBuilder.appendBitZero(); // library

    auto stateInit = stateInitBuilder.intoCell();

    ASSERT_EQ(hex(stateInit->hash), "5a0f742c28067da91e05830f0b072a2069f0617a5f6529d295f6c517d63d67c6");
}

TEST(EverscaleCell, UnalignedRead) {
    CellBuilder dataBuilder;
    dataBuilder.appendU32(0x12312312);

    auto cell = dataBuilder.intoCell();
    auto slice = CellSlice(cell.get());

    slice.dataOffset += 1;
    const auto nextBytes = slice.getNextBytes(2);
    ASSERT_TRUE(nextBytes.size() == 2 && nextBytes[0] == 0x24 && nextBytes[1] == 0x62);
}

TEST(EverscaleCell, ReadZeroBytes) {
    CellBuilder dataBuilder;
    dataBuilder.appendU32(0x12312312);

    auto cell = dataBuilder.intoCell();
    auto slice = CellSlice(cell.get());

    ASSERT_EQ(slice.getNextBytes(0), Data{});
}

TEST(EverscaleCell, InvalidBuilderData) {
    CellBuilder dataBuilder;
    ASSERT_ANY_THROW(dataBuilder.appendRaw(Data{}, 1));
}

TEST(EverscaleCell, DataOverflow) {
    CellBuilder dataBuilder;

    Data data(128, 0x00);
    ASSERT_ANY_THROW(dataBuilder.appendRaw(data, data.size() * 8));
}

TEST(EverscaleCell, DataUnderflow) {
    CellBuilder dataBuilder;
    dataBuilder.appendU32(0x12312312);

    auto cell = dataBuilder.intoCell();
    auto slice = CellSlice(cell.get());

    ASSERT_ANY_THROW(slice.getNextBytes(100));
}

}
