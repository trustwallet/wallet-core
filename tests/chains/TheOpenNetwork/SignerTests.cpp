// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    transfer.set_bounceable(true);

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
    transfer.set_bounceable(true);

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
    transfer.set_bounceable(true);

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
    transfer.set_bounceable(false);

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
    transfer.set_bounceable(true);

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
    transfer.set_bounceable(true);

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
    transfer.set_bounceable(true);

    const auto privateKey = parse_hex("63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    ASSERT_EQ(output.error(), 22);
}

TEST(TheOpenNetworkSigner, JettonTransfer) {
    auto input = Proto::SigningInput();
    auto& jettonTransfer = *input.mutable_jetton_transfer();
    auto& transferData = *jettonTransfer.mutable_transfer();

    transferData.set_wallet_version(Proto::WALLET_V4_R2);
    transferData.set_dest("EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja");
    transferData.set_amount(100 * 1000 * 1000); 
    transferData.set_mode(Proto::SendMode::PAY_FEES_SEPARATELY | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transferData.set_expire_at(1787693046);
    transferData.set_bounceable(true);
    jettonTransfer.set_response_address("EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk"); // send unused toncoins back to sender
    jettonTransfer.set_to_owner("EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8");
    jettonTransfer.set_query_id(69);
    jettonTransfer.set_forward_amount(1);
    jettonTransfer.set_jetton_amount(1000 * 1000 * 1000); // transfer 1 testtwt (decimal precision is 9)

    const auto privateKey = parse_hex("c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(CommonTON::Cell::fromBase64(output.encoded())->hash), "3e4dac37acdc99ca670b3747ab2730e818727d9d25c80d3987abe501356d0da0");
    
    // tx: https://testnet.tonscan.org/tx/2HOPGAXhez3v6sdfj-5p8mPHX4S4T0CgxVbm0E2swxE=
    ASSERT_EQ(output.encoded(), "te6ccgICABoAAQAABCMAAAJFiAC0UQZVyBNtT/W+jqQKnhYasPiDIdSWnNgo1FPyLHxLKh4ABAABAZz3iNHD1z2mxbtpFAtmbVevYMnB4yHPkF3WAsL3KHcrqCw0SWezOg4lVz1zzSReeFDx98ByAqY9+eR5VF3xyugAKamjF/////8AAAAAAAMAAgFoYgAxNB+Hnam4Pt4pSYNuGp+1rhx1QxEXrrZTGnfPOq6D8yAvrwgAAAAAAAAAAAAAAAAAAQADAKoPin6lAAAAAAAAAEVDuaygCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAgE0AAYABQBRAAAAACmpoxfOamBhePRNnx/pqQViBzW0dDCy/+1WLV1VhgbVTL6i30ABFP8A9KQT9LzyyAsABwIBIAANAAgE+PKDCNcYINMf0x/THwL4I7vyZO1E0NMf0x/T//QE0VFDuvKhUVG68qIF+QFUEGT5EPKj+AAkpMjLH1JAyx9SMMv/UhD0AMntVPgPAdMHIcAAn2xRkyDXSpbTB9QC+wDoMOAhwAHjACHAAuMAAcADkTDjDQOkyMsfEssfy/8ADAALAAoACQAK9ADJ7VQAbIEBCNcY+gDTPzBSJIEBCPRZ8qeCEGRzdHJwdIAYyMsFywJQBc8WUAP6AhPLassfEss/yXP7AABwgQEI1xj6ANM/yFQgR4EBCPRR8qeCEG5vdGVwdIAYyMsFywJQBs8WUAT6AhTLahLLH8s/yXP7AAIAbtIH+gDU1CL5AAXIygcVy//J0Hd0gBjIywXLAiLPFlAF+gIUy2sSzMzJc/sAyEAUgQEI9FHypwICAUgAFwAOAgEgABAADwBZvSQrb2omhAgKBrkPoCGEcNQICEekk30pkQzmkD6f+YN4EoAbeBAUiYcVnzGEAgEgABIAEQARuMl+1E0NcLH4AgFYABYAEwIBIAAVABQAGa8d9qJoQBBrkOuFj8AAGa3OdqJoQCBrkOuF/8AAPbKd+1E0IEBQNch9AQwAsjKB8v/ydABgQEI9ApvoTGAC5tAB0NMDIXGwkl8E4CLXScEgkl8E4ALTHyGCEHBsdWe9IoIQZHN0cr2wkl8F4AP6QDAg+kQByMoHy//J0O1E0IEBQNch9AQwXIEBCPQKb6Exs5JfB+AF0z/IJYIQcGx1Z7qSODDjDQOCEGRzdHK6kl8G4w0AGQAYAIpQBIEBCPRZMO1E0IEBQNcgyAHPFvQAye1UAXKwjiOCEGRzdHKDHrFwgBhQBcsFUAPPFiP6AhPLassfyz/JgED7AJJfA+IAeAH6APQEMPgnbyIwUAqhIb7y4FCCEHBsdWeDHrFwgBhQBMsFJs8WWPoCGfQAy2kXyx9SYMs/IMmAQPsABg==");
}

TEST(TheOpenNetworkSigner, JettonTransferComment) {
    auto input = Proto::SigningInput();
    auto& jettonTransfer = *input.mutable_jetton_transfer();
    auto& transferData = *jettonTransfer.mutable_transfer();

    transferData.set_wallet_version(Proto::WALLET_V4_R2);
    transferData.set_dest("EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja");
    transferData.set_amount(100 * 1000 * 1000);
    transferData.set_sequence_number(1);
    transferData.set_mode(Proto::SendMode::PAY_FEES_SEPARATELY | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS);
    transferData.set_expire_at(1787693046);
    transferData.set_comment("test comment");
    transferData.set_bounceable(true);
    jettonTransfer.set_jetton_amount(500 * 1000 * 1000); // transfer 0.5 testtwt (decimal precision is 9)
    jettonTransfer.set_to_owner("EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8");
    jettonTransfer.set_response_address("EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk"); // send unused toncoins back to sender
    jettonTransfer.set_forward_amount(1);
    

    const auto privateKey = parse_hex("c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(CommonTON::Cell::fromBase64(output.encoded())->hash), "c98c205c8dd37d9a6ab5db6162f5b9d37cefa067de24a765154a5eb7a359f22f");
    
    // tx: https://testnet.tonscan.org/tx/Er_oT5R3QK7D-qVPBKUGkJAOOq6ayVls-mgEphpI9Ck=
    // comment can be seen here: https://testnet.tonviewer.com/transaction/12bfe84f947740aec3faa54f04a50690900e3aae9ac9596cfa6804a61a48f429
    ASSERT_EQ(output.encoded(), "te6ccgICAAQAAQAAARgAAAFFiAC0UQZVyBNtT/W+jqQKnhYasPiDIdSWnNgo1FPyLHxLKgwAAQGcaIWVosi1XnveAmoG9y0/mPeNUqUu7GY76mdbRAaVeNeDOPDlh5M3BEb26kkc6XoYDekV60o2iOobN+TGS76jBSmpoxdqjgf2AAAAAQADAAIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEAAwDKD4p+pQAAAAAAAAAAQdzWUAgAC4GWcwteHQM+mcQoV+aZ+myCd1jYqUiawhCzuxMdEzkAFoogyrkCban+t9HUgVPCw1YfEGQ6ktObBRqKfkWPiWVCAgAAAAB0ZXN0IGNvbW1lbnQ=");
}

} // namespace TW::TheOpenNetwork::tests
