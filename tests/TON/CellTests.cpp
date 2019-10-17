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
        ASSERT_EQ("bf7cbe09d71a1bcc373ab9a764917f730a6ed951ffa1a7399b7abd8f8fd73cb4", hex(s.hash()));
    }
    {
        Slice s = Slice::createFromHex("00000000F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
        ASSERT_EQ("3f8e0f65857d989321be21d1b55a01c47b5d5da6b463292841302d4b8a2a39e5", hex(s.hash()));
    }
    {
        Slice s = Slice::createFromBitsStr("31", 5);
        ASSERT_EQ(1, s.size());
        ASSERT_EQ(5, s.sizeBits());
        ASSERT_EQ("34", s.asBytesStr()); // unused bits: 100
        ASSERT_EQ("4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a", hex(s.hash()));
    }
}

TEST(TONCell, CellSimple)
{
    {
        Cell cell;
        cell.setSliceBytesStr("123456");
        ASSERT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(cell.hash()));
    }
    {
        Cell cell;
        cell.setSliceBytesStr("FEDCBA");
        ASSERT_EQ("53a96fa8e030c2c8be0f32cba5a929ca89b5650b699c3c305150a9b8d9669176", hex(cell.hash()));
    }
    {
        Cell cell;
        cell.setSliceBytesStr("00000000F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
        ASSERT_EQ(32 + 4, cell.getSlice().size());
        ASSERT_EQ("3a2e770059fea3557f95c7a269eb51f1675339fed6b0be624ad8b9e649791e1f", hex(cell.hash()));
    }
    {
        Cell cell;
        cell.setSliceBytesStr("FF0020DDA4F260810200D71820D70B1FED44D0D31FD3FFD15112BAF2A122F901541044F910F2A2F80001D31F3120D74A96D307D402FB00DED1A4C8CB1FCBFFC9ED54");
        ASSERT_EQ(66, cell.getSlice().size());
        ASSERT_EQ("a0cfc2c48aee16a271f2cfc0b7382d81756cecb1017d077faaab3bb602f6868c", hex(cell.hash()));
    }
    {
        Cell cell;
        cell.setSliceBytesStr("FF0020DDA4F260810200D71820D70B1FED44D0D7091FD709FFD15112BAF2A122F901541044F910F2A2F80001D7091F3120D74A97D70907D402FB00DED1A4C8CB1FCBFFC9ED54");
        ASSERT_EQ(70, cell.getSlice().size());
        ASSERT_EQ("78ad9f4d84126ddade8c3fa1d3d5fa3304b60c5600ea9f0296419cbf73cda9c4", hex(cell.hash()));
    }
    {
        // Hash of empty cell
        Cell cell;
        ASSERT_EQ("96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7", hex(cell.hash()));
    }
}

TEST(TONCell, CellBits)
{
    {
        // Hash of cell with bits
        Cell cell;
        cell.setSliceBitsStr("31", 5); // 5 bits are 00110, the last 3 are not used
        ASSERT_EQ("34", cell.getSlice().asBytesStr()); // the 5 bits padded with 100
        ASSERT_EQ(1, cell.getSlice().size());
        ASSERT_EQ(5, cell.getSlice().sizeBits());
        ASSERT_EQ("ea23ba20e2f88a07af38948bfaef741741f5a464df43e87067c901e537d1c44f", hex(cell.hash()));
    }
}

TEST(TONCell, CellWithChild)
{
    {
        // cell with one child
        auto c1 = std::make_shared<Cell>();
        c1->setSliceBytesStr("123456");
        ASSERT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(c1->hash()));
        Cell c;
        ASSERT_EQ("96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7", hex(c.hash())); // empty cell
        c.addCell(c1);
        ASSERT_EQ("993e6d53d70fb05f052ce45ac751e24abd7d43e358b297694cfc19bbc796141c", hex(c.hash()));
    }
    {
        // cell with two children
        auto c1 = std::make_shared<Cell>();
        c1->setSliceBytesStr("123456");
        ASSERT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(c1->hash()));
        auto c2 = std::make_shared<Cell>();
        c2->setSliceBytesStr("FEDCBA");
        ASSERT_EQ("53a96fa8e030c2c8be0f32cba5a929ca89b5650b699c3c305150a9b8d9669176", hex(c2->hash()));
        Cell c;
        ASSERT_EQ("96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7", hex(c.hash())); // empty cell
        c.addCell(c1);
        ASSERT_EQ("993e6d53d70fb05f052ce45ac751e24abd7d43e358b297694cfc19bbc796141c", hex(c.hash()));
        c.addCell(c2);
        ASSERT_EQ(2, c.cell_count());
        ASSERT_EQ("3959cba26c91a21d80b6953dd646ce8b8fb3caa507ea866ee98ff92b0230d0b9", hex(c.hash()));
    }
    {
        // cell with slice value and one child
        auto c1 = std::make_shared<Cell>();
        c1->setSliceBytesStr("FEDCBA");
        ASSERT_EQ("53a96fa8e030c2c8be0f32cba5a929ca89b5650b699c3c305150a9b8d9669176", hex(c1->hash()));
        Cell c;
        ASSERT_EQ("96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7", hex(c.hash())); // empty cell
        c.setSliceBytesStr("123456");
        ASSERT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(c.hash()));
        c.addCell(c1);
        ASSERT_EQ("45d4770b7e9816f062cb8e3f5c58e8ed16443b2387a1c6f59b777dfb005822fa", hex(c.hash()));
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

TEST(TONCell, CellStateInit)
{
    {
        // StateInit, with bits slice value and two children
        Cell c;
        c.setSliceBitsStr("34", 5);
        auto ccode = std::make_shared<Cell>();
        ccode->setSliceBytesStr("FF0020DDA4F260810200D71820D70B1FED44D0D7091FD709FFD15112BAF2A122F901541044F910F2A2F80001D7091F3120D74A97D70907D402FB00DED1A4C8CB1FCBFFC9ED54");
        auto cdata = std::make_shared<Cell>();
        cdata->setSliceBytesStr("00000000F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
        c.addCell(ccode);
        c.addCell(cdata);
        ASSERT_EQ("60c04141c6a7b96d68615e7a91d265ad0f3a9a922e9ae9c901d4fa83f5d3c0d0", hex(c.hash()));
    }
}
