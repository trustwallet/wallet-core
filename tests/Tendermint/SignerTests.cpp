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
    input.set_chain_id("test-chain-id");
    input.set_memo("");
    input.set_sequence(0);

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
    amountOfFee->set_denom("photino");
    amountOfFee->set_amount(1);
    *input.mutable_fee() = fee;

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = signer.signInBase64();

    ASSERT_EQ("KEx6ex0DZJ9XG013PJGa6INpBeV6SsAOkItx4ezb9NdVSwJohWqXKrcpM4bhJZuqweoBSoxW3agJrQPHSQAgAQ==", signatureInBase64);

    auto tx = signer.buildTransaction();

    ASSERT_EQ("{\"type\":\"auth/StdTx\",\"value\":{\"fee\":{\"amount\":[{\"amount\":\"1\",\"denom\":\"photino\"}],\"gas\":\"200000\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgSend\",\"value\":{\"amount\":[{\"amount\":\"1\",\"denom\":\"muon\"}],\"from_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"to_address\":\"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"KEx6ex0DZJ9XG013PJGa6INpBeV6SsAOkItx4ezb9NdVSwJohWqXKrcpM4bhJZuqweoBSoxW3agJrQPHSQAgAQ==\"}]}}", tx);
}

}
