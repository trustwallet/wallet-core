// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::Cosmos {

TEST(CosmosSigner, SignTx) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(8);

    auto fromAddress = TW::Cosmos::Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = TW::Cosmos::Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto &message = *input.mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("muon");
    amountOfTx->set_amount(1);
    
    auto &fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(200);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = signer.signInBase64();

    ASSERT_EQ("/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ(
        "{"
            "\"type\":\"auth/StdTx\","
            "\"value\":{"
                "\"fee\":{"
                    "\"amount\":["
                                    "{\"amount\":\"200\",\"denom\":\"muon\"}"
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
                                        "\"signature\":\"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg==\""
                                    "}"
                                "]"
            "}"
        "}", 
        output.json());

        ASSERT_EQ("a264747970656a617574682f53746454786576616c7565a463666565a266616d6f756e7481a266616d6f756e74633230306564656e6f6d646d756f6e6367617366323030303030646d656d6f60636d736781a2647479706572636f736d6f732d73646b2f4d736753656e646576616c7565a366616d6f756e7481a266616d6f756e7461316564656e6f6d646d756f6e6c66726f6d5f61646472657373782d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430326a746f5f61646472657373782d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537336a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416c636f6273507a66544e56653775714141736e6445724a416a71706c6e79756461474230662b522b703346697369676e617475726578582f4437346d644947794942332f73517649626f4c54665339503945562f6659477267485a45322f764e6a395836654d36653537473361746c6a4e422b5041426e52773370546b353175586d6843466f70384f2f5a4a673d3d", hex(output.encoded()));

        /*
            the sample tx on testnet 
            https://hubble.figment.network/chains/gaia-13003/blocks/142933/transactions/3A9206598C3D2E75A5EC074FD33EA53EB18EC729357F0965971C1C51F812AEA3?format=json
        */
}

}
