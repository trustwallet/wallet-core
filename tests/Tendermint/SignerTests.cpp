// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include "Tendermint/Address.h"
#include "Tendermint/Signer.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::Cosmos {

TEST(CosmosSigner, SignTx) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13002");
    input.set_memo("");
    input.set_sequence(2);

    auto fromAddress = TW::Tendermint::Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = TW::Tendermint::Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto message = Proto::SendCoinsMessage();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amount();
    amountOfTx->set_denom("muon");
    amountOfTx->set_amount(1);
    *input.mutable_message() = message;
    
    auto fee = Proto::Fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amount();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(1);
    *input.mutable_fee() = fee;

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = signer.signInBase64();

    ASSERT_EQ("UCnIs7gl7opPxYlrhQGqs25ySPIkPgivO1/ROPNUf451/c5QdrFX5unbXSwfjvW9H80c21JYzNrNnqHuGUCSUw==", signatureInBase64);

    auto txInJsonString = signer.buildTransaction();

    ASSERT_EQ(
        "{"
            "\"type\":\"auth/StdTx\","
            "\"value\":{"
                "\"fee\":{"
                    "\"amount\":["
                                    "{\"amount\":\"1\",\"denom\":\"muon\"}"
                                "],"
                    "\"gas\":\"200000\""
                "},"
                "\"memo\":\"\","
                "\"msg\":["
                        "{"
                            "\"type\":\"cosmos-sdk/MsgSend\","
                            "\"value\":{"
                                "\"amount\":["
                                                "{\"amount\":\"1\",\"denom\":\"muon\"}"
                                            "],"
                                "\"from_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\","
                                "\"to_address\":\"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573\""
                            "}"
                        "}"
                    "],"
                "\"signatures\":["
                                    "{"
                                        "\"pub_key\":{"
                                            "\"type\":\"tendermint/PubKeySecp256k1\","
                                            "\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\""
                                        "},"
                                        "\"signature\":\"UCnIs7gl7opPxYlrhQGqs25ySPIkPgivO1/ROPNUf451/c5QdrFX5unbXSwfjvW9H80c21JYzNrNnqHuGUCSUw==\""
                                    "}"
                                "]"
            "}"
        "}", 
        txInJsonString);

        auto txInBytes = signer.build();

        ASSERT_EQ("a264747970656a617574682f53746454786576616c7565a463666565a266616d6f756e7481a266616d6f756e7461316564656e6f6d646d756f6e6367617366323030303030646d656d6f60636d736781a2647479706572636f736d6f732d73646b2f4d736753656e646576616c7565a366616d6f756e7481a266616d6f756e7461316564656e6f6d646d756f6e6c66726f6d5f61646472657373782d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430326a746f5f61646472657373782d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537336a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416c636f6273507a66544e56653775714141736e6445724a416a71706c6e79756461474230662b522b703346697369676e6174757265785855436e497337676c376f705078596c7268514771733235795350496b506769764f312f524f504e55663435312f6335516472465835756e62585377666a7657394838306332314a597a4e724e6e7148754755435355773d3d", hex(txInBytes));

        // the sample tx on testnet https://hubble.figment.network/chains/gaia-13002/blocks/269400/transactions/92D3D8CCE20C164409F91844A08A7FB7A3B700A3CC555C4B1CF51D44777F60F9?format=json
}

}
