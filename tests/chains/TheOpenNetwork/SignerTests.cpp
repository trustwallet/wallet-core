// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"

#include "TheOpenNetwork/Signer.h"
#include "Everscale/CommonTON/Cell.h"

#include <gtest/gtest.h>

namespace TW::TheOpenNetwork::tests {

TEST(TheOpenNetworkSigner, TransferAndDeploy) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_wallet_version(Proto::WALLET_V4_R2);
    transfer.set_dest("EQDYW_1eScJVxtitoBRksvoV9cCYo4uKGWLVNIHB1JqRR3n0");
    transfer.set_amount(10);
    transfer.set_mode(Proto::SendMode::PAY_FEES_SEPARATELY | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transfer.set_expire_at(1671135440);

    const auto privateKey = parse_hex("63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(CommonTON::Cell::fromBase64(output.encoded())->hash), "b3d9462c13a8c67e19b62002447839c386de51415ace3ff6473b1e6294299819");

    // tx: https://tonscan.org/tx/6ZzWOFKZt_m3kZjbwfbATwLaVwmUOdDp0xjhuY7PO3k=
    ASSERT_EQ(output.encoded(), "te6ccgICABoAAQAAA8sAAAJFiADN98eLgHfrkE8l8gmT8X5REpTVR6QnqDhArTbKlVvbZh4ABAABAZznxvGBhoRXhPogxNY8QmHlihJWxg5t6KptqcAIZlVks1r+Z+r1avCWNCeqeLC/oaiVN4mDx/E1+Zhi33G25rcIKamjF/////8AAAAAAAMAAgFiYgBsLf6vJOEq42xW0AoyWX0K+uBMUcXFDLFqmkDg6k1Io4hQAAAAAAAAAAAAAAAAAQADAAACATQABgAFAFEAAAAAKamjF/Qsd/kxvqIOxdAVBzEna7suKGCUdmEkWyMZ74Ez7o1BQAEU/wD0pBP0vPLICwAHAgEgAA0ACAT48oMI1xgg0x/TH9MfAvgju/Jk7UTQ0x/TH9P/9ATRUUO68qFRUbryogX5AVQQZPkQ8qP4ACSkyMsfUkDLH1Iwy/9SEPQAye1U+A8B0wchwACfbFGTINdKltMH1AL7AOgw4CHAAeMAIcAC4wABwAORMOMNA6TIyx8Syx/L/wAMAAsACgAJAAr0AMntVABsgQEI1xj6ANM/MFIkgQEI9Fnyp4IQZHN0cnB0gBjIywXLAlAFzxZQA/oCE8tqyx8Syz/Jc/sAAHCBAQjXGPoA0z/IVCBHgQEI9FHyp4IQbm90ZXB0gBjIywXLAlAGzxZQBPoCFMtqEssfyz/Jc/sAAgBu0gf6ANTUIvkABcjKBxXL/8nQd3SAGMjLBcsCIs8WUAX6AhTLaxLMzMlz+wDIQBSBAQj0UfKnAgIBSAAXAA4CASAAEAAPAFm9JCtvaiaECAoGuQ+gIYRw1AgIR6STfSmRDOaQPp/5g3gSgBt4EBSJhxWfMYQCASAAEgARABG4yX7UTQ1wsfgCAVgAFgATAgEgABUAFAAZrx32omhAEGuQ64WPwAAZrc52omhAIGuQ64X/wAA9sp37UTQgQFA1yH0BDACyMoHy//J0AGBAQj0Cm+hMYALm0AHQ0wMhcbCSXwTgItdJwSCSXwTgAtMfIYIQcGx1Z70ighBkc3RyvbCSXwXgA/pAMCD6RAHIygfL/8nQ7UTQgQFA1yH0BDBcgQEI9ApvoTGzkl8H4AXTP8glghBwbHVnupI4MOMNA4IQZHN0crqSXwbjDQAZABgAilAEgQEI9Fkw7UTQgQFA1yDIAc8W9ADJ7VQBcrCOI4IQZHN0coMesXCAGFAFywVQA88WI/oCE8tqyx/LP8mAQPsAkl8D4gB4AfoA9AQw+CdvIjBQCqEhvvLgUIIQcGx1Z4MesXCAGFAEywUmzxZY+gIZ9ADLaRfLH1Jgyz8gyYBA+wAG");
}

TEST(TheOpenNetworkSigner, TransferOrdinary) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_wallet_version(Proto::WALLET_V4_R2);
    transfer.set_dest("EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q");
    transfer.set_amount(10);
    transfer.set_sequence_number(6);
    transfer.set_mode(Proto::SendMode::PAY_FEES_SEPARATELY | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transfer.set_expire_at(1671132440);

    const auto privateKey = parse_hex("c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(CommonTON::Cell::fromBase64(output.encoded())->hash), "3908cf8b570c1d3d261c62620c9f368db11f6e821a07614cff64de2e7319f81b");

    // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAALAAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwAA");
}

TEST(TheOpenNetworkSigner, TransferAllBalance) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_wallet_version(Proto::WALLET_V4_R2);
    transfer.set_dest("EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q");
    transfer.set_amount(0);
    transfer.set_sequence_number(7);
    transfer.set_mode(Proto::SendMode::ATTACH_ALL_CONTRACT_BALANCE | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transfer.set_expire_at(1681102222);

    const auto privateKey = parse_hex("c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(CommonTON::Cell::fromBase64(output.encoded())->hash), "d5c5980c9083f697a7f114426effbbafac6d5c88554297d290eb65c8def3008e");

    // tx: https://tonscan.org/tx/cVcXgI9DWNWlN2iyTsteaWJckTswVqWZnRVvX5krXeA=
    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAAK8AAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGc58rMUQc/u78bg+Wtt8ETkyM0udf7S+F7wWk7lnPib2KChnBx9dZ7a/zLzhfLq+W9LjLZZfx995J17+0sbkvGCympoxdkM5WOAAAABwCCAAIBYGIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmAAAAAAAAAAAAAAAAAAQADAAA=");
}

TEST(TheOpenNetworkSigner, TransferAllBalanceNonBounceable) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_wallet_version(Proto::WALLET_V4_R2);
    transfer.set_dest("UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV");
    transfer.set_amount(0);
    transfer.set_sequence_number(8);
    transfer.set_mode(Proto::SendMode::ATTACH_ALL_CONTRACT_BALANCE | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transfer.set_expire_at(1681102222);

    const auto privateKey = parse_hex("c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(CommonTON::Cell::fromBase64(output.encoded())->hash), "e9c816780fa8e578bae309c2e098db8eb16aa25545b3ad2b61bb711ec9562795");

    // tx: https://tonscan.org/tx/0sJkPKu6u6uObVRuSWGd_bVGiyy5lJuzEKDqSXifQEA=
    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAAK8AAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcRQQvxdU1u4QoE2Pas0AsZQMc9lea3+wtSvaC6QfLUlyJ9oISMCFnaErpyFHelDhPu4iuZqhkoLwjkR1VYhFSCimpoxdkM5WOAAAACACCAAIBYEIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmAAAAAAAAAAAAAAAAAAQADAAA=");
}

TEST(TheOpenNetworkSigner, TransferWithASCIIComment) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_wallet_version(Proto::WALLET_V4_R2);
    transfer.set_dest("EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q");
    transfer.set_amount(10);
    transfer.set_sequence_number(10);
    transfer.set_mode(Proto::SendMode::PAY_FEES_SEPARATELY | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transfer.set_expire_at(1681102222);
    transfer.set_comment("test comment");

    const auto privateKey = parse_hex("c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(CommonTON::Cell::fromBase64(output.encoded())->hash), "a8c6943d5587f590c43fcdb0e894046f1965c615e19bcaf0c8407e9ccb74518d");

    // tx: https://tonscan.org/tx/9wjD-VrgEDpa0D9u1g03KSD7kvTNsxRocR7LEdQtCNQ=
    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAAMAAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcY4XlvKqu7spxyjL6vyBSKjbskDgqkHhqBsdTe900RGrzExtpvwc04j94v8HOczEWSMCXjTXk0z+CVUXSL54qCimpoxdkM5WOAAAACgADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwAgAAAAAHRlc3QgY29tbWVudA==");
}

TEST(TheOpenNetworkSigner, TransferWithUTF8Comment) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_wallet_version(Proto::WALLET_V4_R2);
    transfer.set_dest("EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q");
    transfer.set_amount(10);
    transfer.set_sequence_number(11);
    transfer.set_mode(Proto::SendMode::PAY_FEES_SEPARATELY | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transfer.set_expire_at(1681102222);
    transfer.set_comment("тестовый комментарий");

    const auto privateKey = parse_hex("c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(CommonTON::Cell::fromBase64(output.encoded())->hash), "1091dfae81583d3972825633592c24eab0d3d74c91f60fda9d4afe7535103633");

    // tx: https://tonscan.org/tx/VOTt8HW6eRuWHmuM_P3aC-Dy4TMu4cCRePoTAiDfcoQ=
    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAANsAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGchoDa7EdGQuPuehHy3+0X9WNVEvYxdBtaEWn15oYUX8PEKyzztYy94Xq0T2XdhVvj2H7PTSQ+D/Ny1IBRCxk0BimpoxdkM5WOAAAACwADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwBWAAAAANGC0LXRgdGC0L7QstGL0Lkg0LrQvtC80LzQtdC90YLQsNGA0LjQuQ==");
}

TEST(TheOpenNetworkSigner, InvalidWalletVersion) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_wallet_version(Proto::WALLET_V3_R2);
    transfer.set_dest("EQDYW_1eScJVxtitoBRksvoV9cCYo4uKGWLVNIHB1JqRR3n0");
    transfer.set_amount(10);
    transfer.set_mode(Proto::SendMode::PAY_FEES_SEPARATELY | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transfer.set_expire_at(1671135440);

    const auto privateKey = parse_hex("63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    ASSERT_EQ(output.error(), 22);
}

} // namespace TW::TheOpenNetwork::tests
