// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TON/Cell.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::TON;

TEST(TONCell, SliceHash)
{
    {
        Slice s = Slice::createFromHex("123456");
        EXPECT_EQ("bf7cbe09d71a1bcc373ab9a764917f730a6ed951ffa1a7399b7abd8f8fd73cb4", hex(s.hash()));
    }
    {
        Slice s = Slice::createFromHex("00000000F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
        EXPECT_EQ("3f8e0f65857d989321be21d1b55a01c47b5d5da6b463292841302d4b8a2a39e5", hex(s.hash()));
    }
    {
        Slice s = Slice::createFromBitsStr("31", 5);
        EXPECT_EQ(1, s.size());
        EXPECT_EQ(5, s.sizeBits());
        EXPECT_EQ("34", s.asBytesStr()); // unused bits: 100
        EXPECT_EQ("4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a", hex(s.hash()));
    }
}

TEST(TONCell, SliceAppendBits) {
    Slice s;
    EXPECT_EQ(0, s.sizeBits());
    EXPECT_EQ("", s.asBytesStr());
    // byte boundary, append 2 full bytes (8 bits)
    s.appendBits(parse_hex("abcd"), 16);
    EXPECT_EQ(2 * 8, s.sizeBits());
    EXPECT_EQ("abcd", s.asBytesStr());
    // byte boundary, append 5 bits 01100 (plus closing 1)
    s.appendBits(parse_hex("60"), 5);
    EXPECT_EQ(2 * 8 + 5, s.sizeBits());
    EXPECT_EQ("abcd64", s.asBytesStr());
    // append 2 full bytes
    s.appendBits(parse_hex("1234"), 16);
    EXPECT_EQ(4 * 8 + 5, s.sizeBits());
    EXPECT_EQ("abcd6091a4", s.asBytesStr());
    // append 5 bits 00110 (plus closing 1)
    s.appendBits(parse_hex("30"), 5);
    EXPECT_EQ(5 * 8 + 2, s.sizeBits());
    EXPECT_EQ("abcd6091a1a0", s.asBytesStr());
    // append nothing
    s.appendBits(parse_hex(""), 0);
    EXPECT_EQ(5 * 8 + 2, s.sizeBits());
    EXPECT_EQ("abcd6091a1a0", s.asBytesStr());
    // append 1 bit 0
    s.appendBits(parse_hex("00"), 1);
    EXPECT_EQ(5 * 8 + 3, s.sizeBits());
    EXPECT_EQ("abcd6091a190", s.asBytesStr());
    // append 4 bits 0100
    s.appendBits(parse_hex("40"), 4);
    EXPECT_EQ(5 * 8 + 7, s.sizeBits());
    EXPECT_EQ("abcd6091a189", s.asBytesStr());
    // append 2 bits 01
    s.appendBits(parse_hex("40"), 2);
    EXPECT_EQ(6 * 8 + 1, s.sizeBits());
    EXPECT_EQ("abcd6091a188c0", s.asBytesStr());
    // append 7 bits 0110100
    s.appendBits(parse_hex("68"), 7);
    EXPECT_EQ(7 * 8, s.sizeBits());
    EXPECT_EQ("abcd6091a188b4", s.asBytesStr());
}

TEST(TONCell, CellSimple)
{
    {
        Cell cell;
        cell.setSliceBytesStr("123456");
        EXPECT_EQ("123456", cell.getSlice().asBytesStr());
        EXPECT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(cell.hash()));
        EXPECT_EQ(5, cell.serializedOwnSize());
    }
    {
        Cell cell;
        cell.setSliceBytesStr("FEDCBA");
        EXPECT_EQ("53a96fa8e030c2c8be0f32cba5a929ca89b5650b699c3c305150a9b8d9669176", hex(cell.hash()));
        EXPECT_EQ(5, cell.serializedOwnSize());
    }
    {
        Cell cell;
        cell.setSliceBytesStr("00000000F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
        EXPECT_EQ(32 + 4, cell.getSlice().size());
        EXPECT_EQ("3a2e770059fea3557f95c7a269eb51f1675339fed6b0be624ad8b9e649791e1f", hex(cell.hash()));
        EXPECT_EQ(38, cell.serializedOwnSize());
    }
    {
        Cell cell;
        cell.setSliceBytesStr("FF0020DDA4F260810200D71820D70B1FED44D0D31FD3FFD15112BAF2A122F901541044F910F2A2F80001D31F3120D74A96D307D402FB00DED1A4C8CB1FCBFFC9ED54");
        EXPECT_EQ(66, cell.getSlice().size());
        EXPECT_EQ("a0cfc2c48aee16a271f2cfc0b7382d81756cecb1017d077faaab3bb602f6868c", hex(cell.hash()));
        EXPECT_EQ(68, cell.serializedOwnSize());
    }
    {
        Cell cell;
        cell.setSliceBytesStr("FF0020DDA4F260810200D71820D70B1FED44D0D7091FD709FFD15112BAF2A122F901541044F910F2A2F80001D7091F3120D74A97D70907D402FB00DED1A4C8CB1FCBFFC9ED54");
        EXPECT_EQ(70, cell.getSlice().size());
        EXPECT_EQ("78ad9f4d84126ddade8c3fa1d3d5fa3304b60c5600ea9f0296419cbf73cda9c4", hex(cell.hash()));
        EXPECT_EQ(72, cell.serializedOwnSize());
    }
    {
        // Hash of empty cell
        Cell cell;
        EXPECT_EQ("96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7", hex(cell.hash()));
        EXPECT_EQ(2, cell.serializedOwnSize());
    }
}

TEST(TONCell, CellBits)
{
    {
        // Hash of cell with bits
        Cell cell;
        cell.setSliceBitsStr("31", 5); // 5 bits are 00110, the last 3 are not used
        EXPECT_EQ("34", cell.getSlice().asBytesStr()); // the 5 bits padded with 100
        EXPECT_EQ(1, cell.getSlice().size());
        EXPECT_EQ(5, cell.getSlice().sizeBits());
        EXPECT_EQ("ea23ba20e2f88a07af38948bfaef741741f5a464df43e87067c901e537d1c44f", hex(cell.hash()));
        EXPECT_EQ(3, cell.serializedOwnSize());
    }
}

TEST(TONCell, CellWithChild)
{
    {
        // cell with one child
        auto c1 = std::make_shared<Cell>();
        c1->setSliceBytesStr("123456");
        EXPECT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(c1->hash()));
        Cell c;
        EXPECT_EQ("96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7", hex(c.hash())); // empty cell
        c.addCell(c1);
        EXPECT_EQ("993e6d53d70fb05f052ce45ac751e24abd7d43e358b297694cfc19bbc796141c", hex(c.hash()));
        EXPECT_EQ(23, c.serializedSize(Cell::SerializationMode::WithCRC32C));
        Data ser;
        c.serialize(ser, Cell::SerializationMode::WithCRC32C);
        EXPECT_EQ("b5ee9c724101020100080001000100061234567ac0b173", hex(ser));
    }
    {
        // cell with two children
        auto c1 = std::make_shared<Cell>();
        c1->setSliceBytesStr("123456");
        EXPECT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(c1->hash()));
        auto c2 = std::make_shared<Cell>();
        c2->setSliceBytesStr("FEDCBA");
        EXPECT_EQ("53a96fa8e030c2c8be0f32cba5a929ca89b5650b699c3c305150a9b8d9669176", hex(c2->hash()));
        Cell c;
        EXPECT_EQ("96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7", hex(c.hash())); // empty cell
        c.addCell(c1);
        EXPECT_EQ("993e6d53d70fb05f052ce45ac751e24abd7d43e358b297694cfc19bbc796141c", hex(c.hash()));
        c.addCell(c2);
        EXPECT_EQ(2, c.cellCount());
        EXPECT_EQ("3959cba26c91a21d80b6953dd646ce8b8fb3caa507ea866ee98ff92b0230d0b9", hex(c.hash()));
        EXPECT_EQ(29, c.serializedSize(Cell::SerializationMode::WithCRC32C));
        Data ser;
        c.serialize(ser, Cell::SerializationMode::WithCRC32C);
        EXPECT_EQ("b5ee9c7241010301000e000200010200061234560006fedcba50ea05ee", hex(ser));
    }
    {
        // cell with slice value and one child
        auto c1 = std::make_shared<Cell>();
        c1->setSliceBytesStr("FEDCBA");
        EXPECT_EQ("53a96fa8e030c2c8be0f32cba5a929ca89b5650b699c3c305150a9b8d9669176", hex(c1->hash()));
        Cell c;
        EXPECT_EQ("96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7", hex(c.hash())); // empty cell
        c.setSliceBytesStr("123456");
        EXPECT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(c.hash()));
        c.addCell(c1);
        EXPECT_EQ("45d4770b7e9816f062cb8e3f5c58e8ed16443b2387a1c6f59b777dfb005822fa", hex(c.hash()));
        EXPECT_EQ(26, c.serializedSize(Cell::SerializationMode::WithCRC32C));
        Data ser;
        c.serialize(ser, Cell::SerializationMode::WithCRC32C);
        EXPECT_EQ("b5ee9c7241010201000b000106123456010006fedcba7dc78a01", hex(ser));
    }
}

TEST(TONCell, CellError)
{
    {
        Cell cell;
        ASSERT_ANY_THROW({
            cell.setSliceBytesStr(""); // empty data
        });
    }
    {
        Cell cell;
        ASSERT_ANY_THROW({
            cell.setSliceBitsStr("31", 80); // too few bytes for 80 bits
        });
    }
}

TEST(TONCell, CellErrorTooManyCells)
{
    {
        Cell cell;
        auto child = std::make_shared<Cell>();
        for (int i = 0; i < Cell::max_cells; ++i)
            cell.addCell(child);
        ASSERT_ANY_THROW({
            // (N+1)th add fails
            cell.addCell(child);
        });
    }
}

TEST(TONCell, CellStateInit1)
{
    {
        // StateInit, with bits slice value and two children
        Cell c;
        c.setSliceBitsStr("30", 5);
        auto ccode = std::make_shared<Cell>();
        ccode->setSliceBytesStr("FF0020DDA4F260810200D71820D70B1FED44D0D7091FD709FFD15112BAF2A122F901541044F910F2A2F80001D7091F3120D74A97D70907D402FB00DED1A4C8CB1FCBFFC9ED54");
        auto cdata = std::make_shared<Cell>();
        cdata->setSliceBytesStr("00000000F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
        c.addCell(ccode);
        c.addCell(cdata);
        EXPECT_EQ("60c04141c6a7b96d68615e7a91d265ad0f3a9a922e9ae9c901d4fa83f5d3c0d0", hex(c.hash()));
        EXPECT_EQ(130, c.serializedSize(Cell::SerializationMode::WithCRC32C));
        Data ser;
        c.serialize(ser, Cell::SerializationMode::WithCRC32C);
        EXPECT_EQ("b5ee9c72410103010073000201340102008cff0020dda4f260810200d71820d70b1fed44d0d7091fd709ffd15112baf2a122f901541044f910f2a2f80001d7091f3120d74a97d70907d402fb00ded1a4c8cb1fcbffc9ed54004800000000f61cf0bc8e891ad7636e0cd35229d579323aa2da827eb85d8071407464dc2fa3984101af",
            hex(ser));
    }
}

TEST(TONCell, CellStateInit2)
{
    {
        // StateInit, with bits slice value and two children
        Cell c;
        c.setSliceBitsStr("30", 5);
        auto ccode = std::make_shared<Cell>();
        ccode->setSliceBytesStr("FF0020DD2082014C97BA9730ED44D0D70B1FE0A4F260810200D71820D70B1FED44D0D31FD3FFD15112BAF2A122F901541044F910F2A2F80001D31F3120D74A96D307D402FB00DED1A4C8CB1FCBFFC9ED54");
        auto cdata = std::make_shared<Cell>();
        cdata->setSliceBytesStr("0000000037F14C50F6435B11B9326E1218524F7F072D0A5EA8221CCA71682E7D6ED64213");
        c.addCell(ccode);
        c.addCell(cdata);
        EXPECT_EQ("307c9efea683c14f572a9032086ffb8cf8168c5d49094cbffbcbf0bdc3037990", hex(c.hash()));
        EXPECT_EQ(141, c.serializedSize(Cell::SerializationMode::WithCRC32C));
        Data ser;
        c.serialize(ser, Cell::SerializationMode::WithCRC32C);
        EXPECT_EQ("b5ee9c7241010301007e00020134010200a2ff0020dd2082014c97ba9730ed44d0d70b1fe0a4f260810200d71820d70b1fed44d0d31fd3ffd15112baf2a122f901541044f910f2a2f80001d31f3120d74a96d307d402fb00ded1a4c8cb1fcbffc9ed5400480000000037f14c50f6435b11b9326e1218524f7f072d0a5ea8221cca71682e7d6ed6421381c553bd",
            hex(ser));
    }
}