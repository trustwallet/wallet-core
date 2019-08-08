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

TEST(CoinexStaking, Staking) {
    auto input = Proto::SigningInput();
    input.set_account_number(474);
    input.set_chain_id("coinexdex-test");
    input.set_memo("ifwallet");
    input.set_sequence(1);

    auto& message = *input.mutable_stake_message();
    message.set_delegator_address("coinex1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0juwxvr");
    message.set_validator_address("coinexvaloper1pxf3sgcxpu99e4havmeus7gmmt8u3j9jpv7j4d");
    auto& amountOfTx = *message.mutable_amount();
    amountOfTx.set_denom("cet");
    amountOfTx.set_amount(1000000);

    auto &fee = *input.mutable_fee();
    fee.set_gas(100000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(2000000);

    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = signer.signInBase64();

    auto output = signer.build();

    ASSERT_EQ("{\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"2000000\",\"denom\":\"cet\"}],\"gas\":\"100000\"},\"memo\":\"ifwallet\",\"msg\":[{\"type\":\"cosmos-sdk/MsgDelegate\",\"value\":{\"amount\":{\"amount\":\"1000000\",\"denom\":\"cet\"},\"delegator_address\":\"coinex1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0juwxvr\",\"validator_address\":\"coinexvaloper1pxf3sgcxpu99e4havmeus7gmmt8u3j9jpv7j4d\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AkvCoxJlFT8H5w4LqwhyTmuF4hf4zWKM62KXQke7STOC\"},\"signature\":\"WTHDn9sbBCL0HMfASK8FrHPW3Yrb9ks1YpmcVb4OlRhFeTnQJPVrLHAEmeIV1D/MERPbRAy7abQFcO4M509tyw==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

    //ASSERT_EQ("a1627478a563666565a266616d6f756e7481a266616d6f756e74633430306564656e6f6d6363657463676173653130303030646d656d6f68696677616c6c6574636d736781a2647479706576636f736d6f732d73646b2f4d736744656c65676174656576616c7565a366616d6f756e74a266616d6f756e7464313030306564656e6f6d636365747164656c656761746f725f61646472657373782d636f696e657831686b6671337a616861716b6b7a78356d6a6e616d776a73667071326a6b377a306a75777876727176616c696461746f725f616464726573737834636f696e657876616c6f7065723170786633736763787075393965346861766d65757337676d6d743875336a396a7076376a34646a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416b76436f784a6c465438483577344c71776879546d7546346866347a574b4d36324b58516b653753544f43697369676e61747572657858577158537a61713476486b7a423341373161697a6b6474447867665867315057656e7065574e672f767263654b6b6a746637376a394372676b636d483762545643695a495670623952784a77574733754865564674513d3d647479706572636f736d6f732d73646b2f4d736753656e64", hex(output.encoded()));

    // https://testnet.coinex.org/transactions/0ECD0F2930F7EBDD8B8B0B3DCF641E94EC0A3C94539EFF14C9D828B6F31FF055
}

}
