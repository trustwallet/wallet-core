// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/Common.pb.h"
#include "proto/Ripple.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

namespace TW::Ripple::tests {

TEST(TWAnySignerRipple, SignXrpPayment) {
    // https://testnet.xrpl.org/transactions/A202034796F37F38D1D20F2025DECECB1623FC801F041FC694199C0D0E49A739
    auto key = parse_hex("a5576c0f63da10e584568c8d134569ff44017b0a249eb70657127ae04f38cc77");
    Proto::SigningInput input;

    input.mutable_op_payment()->set_amount(10);
    input.set_fee(10);
    input.set_sequence(32268248);
    input.set_last_ledger_sequence(32268269);
    input.set_account("rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq");
    input.mutable_op_payment()->set_destination("rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "12000022000000002401ec5fd8201b01ec5fed61400000000000000a68400000000000000a732103d13e1152965a51a4a9fd9a8b4ea3dd82a4eba6b25fcad5f460a2342bb650333f74463044022037d32835c9394f39b2cfd4eaf5b0a80e0db397ace06630fa2b099ff73e425dbc02205288f780330b7a88a1980fa83c647b5908502ad7de9a44500c08f0750b0d9e8481144c55f5a78067206507580be7bb2686c8460adff983148132e4e20aecf29090ac428a9c43f230a829220d");

    // invalid tag
    input.mutable_op_payment()->set_destination_tag(641505641505);

    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(output.error(), Common::Proto::SigningError::Error_invalid_memo);
    EXPECT_EQ(output.encoded(), "");
}

TEST(TWAnySignerRipple, SignXrpPaymentMain) {
    // https://xrpscan.com/tx/4B9D022E8C77D798B7D11C41FDFDCF468F03A5564151C520EECA1E96FF1A1610
    auto key = parse_hex("acf1bbf6264e699da0cc65d17ac03fcca6ded1522d19529df7762db46097ff9f");
    Proto::SigningInput input;

    input.mutable_op_payment()->set_amount(1000000);
    input.set_fee(10);
    input.set_sequence(75674534);
    input.set_last_ledger_sequence(75674797);
    input.set_account("rGV1v1xw23PHcRn4Km4tF8R2mfh6yTZkcP");
    input.mutable_op_payment()->set_destination("rNLpgsBTCwiaZAnHe2ZViAN1GcXZtYW6rg");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "1200002200000000240482b3a6201b0482b4ad6140000000000f424068400000000000000a7321027efc5f15071d2ae5e73ee09a0c17456c5d9170a41d67e3297c554829199be80b74473045022100e1c746c3aeebc8278c627ee4c2ce5cae97e3856292c7fe5388f803920230a37b02207d2eccb76cd35dd379d6b24c2cabd786e62d34a564cf083e863176109c5b6bb48114aa000c09c692ef1f82787e51e22833149941ea2083149232ef60695add51f0f84534cc4084e4fdfc698e");
}

TEST(TWAnySignerRipple, SignTrustSetPayment) {
    // https://testnet.xrpl.org/transactions/31042345374CFF785B3F7E2A3716E3BAB7E2CAA30D40F5E488E67ABA116655B9
    auto key = parse_hex("8753e78ee2963f301f82e5eeab2754f593fc242ce94273dd2fb0684e3b0f2b91");
    Proto::SigningInput input;

    input.mutable_op_trust_set()->mutable_limit_amount()->set_currency("USD");
    input.mutable_op_trust_set()->mutable_limit_amount()->set_value("10");
    input.mutable_op_trust_set()->mutable_limit_amount()->set_issuer("rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn");
    input.set_fee(10);
    input.set_sequence(32268473);
    input.set_last_ledger_sequence(32268494);
    input.set_account("rnRkLPni2Q5yMxSqyJSJEkKUfQNFkaAspS");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "12001422000000002401ec60b9201b01ec60ce63d4c38d7ea4c6800000000000000000000000000055534400000000004b4e9c06f24296074f7bc48f92a97916c6dc5ea968400000000000000a732103dc4a0dae2d550de7cace9c26c1a331a114e3e7efee5577204b476d27e2dc683a7446304402206ebcc7a689845df373dd2566cd3789862d426d9ad4e6a09c2d2772b57e82696a022066b1f217a0f0d834d167613a313f74097423a9ccd11f1ae7f90ffab0d2fc26b58114308ea8e515b64f2e6616a33b42e1bbb9fa00bbd2");
}

TEST(TWAnySignerRipple, SignTokenPayment0) {
    // https://testnet.xrpl.org/transactions/8F7820892294598B58CFA2E1101D15ED98C179B25A2BA6DAEB4F5B727CB00D4E
    auto key = parse_hex("4ba5fd2ebf0f5d7e579b3c354c263ebb39cda4093845125786a280301af14e21");
    Proto::SigningInput input;
    input.mutable_op_payment()->mutable_currency_amount()->set_currency("USD");
    input.mutable_op_payment()->mutable_currency_amount()->set_value("10");
    input.mutable_op_payment()->mutable_currency_amount()->set_issuer("rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn");
    input.set_fee(10);
    input.set_sequence(32268645);
    input.set_last_ledger_sequence(32268666);
    input.set_account("raPAA61ca99bdwNiZs5JJukR5rvkHWvkBX");
    input.mutable_op_payment()->set_destination("rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF");
    input.set_private_key(key.data(), key.size());
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);
    EXPECT_EQ(hex(output.encoded()), "12000022000000002401ec6165201b01ec617a61d4c38d7ea4c6800000000000000000000000000055534400000000004b4e9c06f24296074f7bc48f92a97916c6dc5ea968400000000000000a7321020652a477b0cca8b74d6e68a6a386a836b226101617481b95180eaffbe841b3227446304402203e925caeb05006afb135254e9ae4e46de2019db6c6f68614ef969885063a777602206af110fc29775256fcad8b14974c6a838141d82193192d3b57324fe1079afa1781143b2fa4f36553e5b7a4f54ff9e6883e44b4b0dbb383148132e4e20aecf29090ac428a9c43f230a829220d");
}

TEST(TWAnySignerRipple, SignTokenPayment1) {
    // https://testnet.xrpl.org/transactions/14606DAAFA54DB29B738000DFC133312B341FFC1D22D57AE0C8D54C9C56E19D8
    auto key = parse_hex("4041882ce8c2ceea6f4cfe1a067b927c1e1eb2f5eb025eaf2f429479a7ec3738");
    Proto::SigningInput input;

    input.mutable_op_payment()->mutable_currency_amount()->set_currency("USD");
    input.mutable_op_payment()->mutable_currency_amount()->set_value("29.3e-1");
    input.mutable_op_payment()->mutable_currency_amount()->set_issuer("rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn");
    input.set_fee(10);
    input.set_sequence(32268768);
    input.set_last_ledger_sequence(32268789);
    input.set_account("raJe5XVt99649qn5Pg7cKdmgEYdN3d4Mky");
    input.mutable_op_payment()->set_destination("rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "12000022000000002401ec61e0201b01ec61f561d48a68d1c931200000000000000000000000000055534400000000004b4e9c06f24296074f7bc48f92a97916c6dc5ea968400000000000000a73210348c331ab218ba964150490c83875b06ccad2100b1f5707f296764712738cf1ca74473045022100a938783258d33e2e3e6099d1ab68fd85c3fd21adfa00e136a67bed8fddec6c9a02206cc6784c1f212f19dc939207643d361ceaa8334eb366722cf33b24dc7669dd7a81143a2f2f189d05abb8519cc9dee0e2dbc6fa53924183148132e4e20aecf29090ac428a9c43f230a829220d");
}

TEST(TWAnySignerRipple, SignNfTokenBurn) {
    // https://devnet.xrpl.org/transactions/37DA90BE3C30016B3A2C3D47D9677278A3F6D4141B318793CE6AA467A6530E2D
    auto key = parse_hex("7c2ea5c7b1fd7dfc62d879918b7fc779cdff6bf6391d02ec99854297e916318e");
    Proto::SigningInput input;

    input.mutable_op_nftoken_burn()->set_nftoken_id("000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65");
    input.set_fee(10);
    input.set_sequence(22858395);
    input.set_last_ledger_sequence(22858416);
    input.set_account("rhR1mTXkg4iSGhz7zsEKBLbV3MkopivPVF");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "12001a220000000024015cca9b201b015ccab05a000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d6568400000000000000a73210254fc876043109af1ff11b832320be4436ef51dcc344da5970c9b6c6d1fbcddcf744730450221008b4d437bc92aa4643b275b17c0f88a1bef2c1c160ece5faf93b03e2d31b8278602207640e7e35426352deaafecf61e2b401a4ea1fc645839280370a72fa3c41aea7d8114259cbcf9635360bc302f27d0ce72c18d4dbe9c8d");
}

TEST(TWAnySignerRipple, SignNfTokenCreateOffer) {
    // https://devnet.xrpl.org/transactions/E61D66E261DB89CEAAB4F54ECF792B329296CB524E8B40EA99D27CF5E16DD27D
    auto key = parse_hex("1963884da4a4da79ad7681d106b2c55fb652c68ca7b288dd12bb86cd40b9d940");
    Proto::SigningInput input;

    input.mutable_op_nftoken_create_offer()->set_nftoken_id("000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65");
    input.mutable_op_nftoken_create_offer()->set_destination("rDxTa8vhigDUCq9nmZY8jAkFne5XrcYbxG");
    input.set_fee(10);
    input.set_sequence(22857522);
    input.set_last_ledger_sequence(22857543);
    input.set_account("rJdxtrVoL3Tak74EzN8SdMxFF6RP9smjJJ");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "12001b220000000124015cc732201b015cc7475a000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d6561400000000000000068400000000000000a7321022707066e4f8b87b749ef802338be064065dc978f0ea52ea9c8c8ea0a6145571974473045022100a148140469b8e9e2f9aa43631f3101e532d161d49a05e739cd3494ea208bd657022029a9752df3fc0d23b8fdb46d2274e69ab198ce6f373aeb7cdd0d81ab05aff6f48114c177c23ed1f5d175f42fd7970ece74ac18d61c4d83148e1e2ca343165bf30e96abead961f7a34510ad93");
}

TEST(TWAnySignerRipple, SignNfTokenAcceptOffer) {
    // https://devnet.xrpl.org/transactions/6BB00A7BABB8797D60E3AB0E52DB64562524D014833977D87B04CA9FA3F56AD7
    auto key = parse_hex("3c01b3458d2b2a4b86a5699d11682d791b5c3136692c5594f7a8ca7f3967e7ae");
    Proto::SigningInput input;

    input.mutable_op_nftoken_accept_offer()->set_sell_offer("000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65");
    input.set_fee(10);
    input.set_sequence(22857743);
    input.set_last_ledger_sequence(22857764);
    input.set_account("rPa2KsEuSuZnmjosds99nhgsoiKtw85j6Z");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "12001d220000000024015cc80f201b015cc824501d000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d6568400000000000000a73210331c298cb86428b9126bd4af6a952870cfe3fe5065dc093cf97f3edbb27e9dd15744630440220797922caaa593c4e91fa6b63a38c92ef9f5e2183128918dda166f4292882e137022057702b668d7463ef1d01dad5ee6633bd36f0aa358dacc90d6b68d248672a400f8114f260a758132d3ed27e52d7f55ef0481606f090d4");
}

TEST(TWAnySignerRipple, SignNfTokenCancelOffer) {
    // https://devnet.xrpl.org/transactions/CBA148308A0D1561E5E8CDF1F2E8D5562C320C221AC4053AA5F495CEF4B5D5D4
    auto key = parse_hex("3e50cc102d8c96abd55f047a536b6425154514ba8abdf5f09335a7c644176c5d");
    Proto::SigningInput input;

    input.mutable_op_nftoken_cancel_offer()->add_token_offers("000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65");
    input.set_fee(10);
    input.set_sequence(22857838);
    input.set_last_ledger_sequence(22857859);
    input.set_account("rPqczdU9bzow966hQKQejsXrMJspM7G4CC");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "12001c220000000024015cc86e201b015cc88368400000000000000a7321022250f103fd045edf2e552df2d20aca01a52dc6aedd522d68767f1c744fedb39d74463044022015fff495fc5d61cd71e5815e4d23845ec26f4dc94adb85207feba2c97e19856502207297ec84afc0bb74aa8a20d7254025a82d9b9f177f648845d8c72ee62884ff618114fa84c77f2a5245ef774845d40428d2a6f9603415041320000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65");
}

} // namespace TW::Ripple::tests
