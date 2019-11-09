// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"
#include <iostream>

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
    auto signatureInBase64 = Base64::encode(signature);

    ASSERT_EQ("/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"200\",\"denom\":\"muon\"}],\"gas\":\"200000\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgSend\",\"value\":{\"amount\":[{\"amount\":\"1\",\"denom\":\"muon\"}],\"from_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"to_address\":\"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

    ASSERT_EQ(hex(output.signature()), "fc3ef899d206c88077fec42f21ba0b4df4bd3fd115fdf606ae01d9136fef363f57e9e33a7b9ec6ddab658cd07e3c0067470de94e4e75b979a1085a29f0efd926");


    /*
        the sample tx on testnet
        https://hubble.figment.network/chains/gaia-13003/blocks/142933/transactions/3A9206598C3D2E75A5EC074FD33EA53EB18EC729357F0965971C1C51F812AEA3?format=json
    */
}

}
