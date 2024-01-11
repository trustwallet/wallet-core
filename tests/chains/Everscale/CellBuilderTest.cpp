// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "BinaryCoding.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include "Everscale/CommonTON/Cell.h"
#include "Everscale/CommonTON/CellBuilder.h"
#include "Everscale/Wallet.h"

#include <gtest/gtest.h>

using boost::multiprecision::uint128_t;

namespace TW::Everscale {

void checkBuilder(const uint128_t& value, uint16_t bitLen, const std::string& hash) {
    CellBuilder dataBuilder;
    dataBuilder.appendU128(value);
    const auto cell = dataBuilder.intoCell();
    ASSERT_EQ(cell->bitLen, bitLen);
    ASSERT_EQ(hex(cell->hash), hash);
}

TEST(EverscaleCell, BuilderVarUint16) {
    const uint128_t oneEver{1'000'000'000u};

    checkBuilder(0, 4, "5331fed036518120c7f345726537745c5929b8ea1fa37b99b2bb58f702671541");
    checkBuilder(1, 12, "d46edee086ccbace01f45c13d26d49b68f74cd1b7616f4662e699c82c6ec728b");
    checkBuilder(255, 12, "bd16b2d60c93163fbed832e91a5faec484715c48176857c57dcedf9f6e0f32f6");
    checkBuilder(256, 20, "16559011ce6f0f7aaa765179e73ef293f39610f5baa3838a1dc8c52da95793b3");
    checkBuilder(oneEver, 36, "e139b2d96d0bd76da98c3c23b0dc0481dcfe19562798fefbb7bf2e56d8ef37b5");
    checkBuilder(10 * oneEver, 44, "8882fead71f2deb3aa7b8dbd15bbb42c651fcaae8da82e6d5cf8e49825eed12b");
    checkBuilder(1000000 * oneEver, 60, "125f2f85da07f9d92148c067bc19aecbf4da65becdd6b51f17ae3a2aeb2c1bdd");
    checkBuilder(1'000'000'000'000u * oneEver, 76, "39bcb314cdb31de5159764d9c28779de27be44210ffcc52a27aa01bff1d82bf7");
}

TEST(EverscaleCell, ComputeContractAddress) {
    const auto seqno = 0;
    const auto walletId = WALLET_ID;
    const auto publicKey = PublicKey(parse_hex("7dbe83e9b223157e85bed2628430e2cdb531d5c99ab428618b7dd29b567a0369"), TWPublicKeyTypeED25519);

    CellBuilder dataBuilder;
    dataBuilder.appendU32(seqno);
    dataBuilder.appendU32(walletId);
    dataBuilder.appendRaw(publicKey.bytes, 256);

    const auto data = dataBuilder.intoCell();

    // Builder should be empty after `intoCell`
    {
        const auto emptyCell = dataBuilder.intoCell();
        ASSERT_EQ(hex(emptyCell->hash), "96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7");
    }

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

} // namespace TW::Everscale
