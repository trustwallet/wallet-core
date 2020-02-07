// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the // file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Coinex.pb.h"
#include "Cosmos/Address.h"
#include "Coinex/Signer.h"
#include <iostream>

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::Coinex {

TEST(CoinexSigner, SignTx) {
    auto input = Proto::SigningInput();
    input.set_account_number(35);
    input.set_chain_id("coinexdex-test2001");
    input.set_memo("ifwallet");
    input.set_sequence(2);

    auto fromAddress = TW::Cosmos::Address::decode("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j").first;
    auto toAddress = TW::Cosmos::Address::decode("cettest1hpgvshhx4t3mm0t54v5d7xu0qmw675nnqkhv8q").first;

    auto &message = *input.mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    message.set_unlock_time(0);

    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("cet");
    amountOfTx->set_amount(1000000);

    auto &fee = *input.mutable_fee();
    fee.set_gas(100000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(2000000);

    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Coinex::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    auto output = signer.build();

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"2000000\",\"denom\":\"cet\"}],\"gas\":\"100000\"},\"memo\":\"ifwallet\",\"msg\":[{\"type\":\"bankx/MsgSend\",\"value\":{\"amount\":[{\"amount\":\"1000000\",\"denom\":\"cet\"}],\"from_address\":\"cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j\",\"to_address\":\"cettest1hpgvshhx4t3mm0t54v5d7xu0qmw675nnqkhv8q\",\"unlock_time\":\"0\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AkvCoxJlFT8H5w4LqwhyTmuF4hf4zWKM62KXQke7STOC\"},\"signature\":\"zL48DBgg2hkpdf/CWoJ8eEOcZNKMmP3a7rtGZn/G+812IpdEDL6PA3AF7kqeGMwNHPLAArqejpfQOCfqjouiTA==\"}],\"type\":\"bankx/MsgSend\"}}", output.json());

    //ASSERT_EQ("a1627478a563666565a266616d6f756e7481a266616d6f756e74633430306564656e6f6d6363657463676173653130303030646d656d6f68696677616c6c6574636d736781a2647479706572636f736d6f732d73646b2f4d736753656e646576616c7565a366616d6f756e7481a266616d6f756e746531303030306564656e6f6d636365746c66726f6d5f61646472657373782d636f696e657831686b6671337a616861716b6b7a78356d6a6e616d776a73667071326a6b377a306a75777876726a746f5f61646472657373782d636f696e65783170786633736763787075393965346861766d65757337676d6d743875336a396a367261366d656a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416b76436f784a6c465438483577344c71776879546d7546346866347a574b4d36324b58516b653753544f43697369676e61747572657858394f5968306a6f39382b4c726e507263544c5a4b327249736c4d46513741466475564442676252537a34776e764b547179496a4150705a5a6d4d436439634a4d4a44796a416667505a49706351486a515376322f78513d3d647479706572636f736d6f732d73646b2f4d736753656e64", hex(output.encoded()));
    // https://testnet.coinex.org/transactions/781997AA12A723F4C2B5BBFB70F53BCDEB2CB99C97DD3887815152FD110B579E

}

TEST(CoinexSigner, SignTokenTx) {
    auto input = Proto::SigningInput();
    input.set_account_number(35);
    input.set_chain_id("coinexdex-test2001");
    input.set_memo("ifwallet");
    input.set_sequence(4);

    auto fromAddress = TW::Cosmos::Address::decode("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j").first;
    auto toAddress = TW::Cosmos::Address::decode("cettest1hpgvshhx4t3mm0t54v5d7xu0qmw675nnqkhv8q").first;

    auto &message = *input.mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    message.set_unlock_time(0);

    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("ift");
    amountOfTx->set_amount(10000);

    auto &fee = *input.mutable_fee();
    fee.set_gas(100000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(2000000);

    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Coinex::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    auto output = signer.build();

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"2000000\",\"denom\":\"cet\"}],\"gas\":\"100000\"},\"memo\":\"ifwallet\",\"msg\":[{\"type\":\"bankx/MsgSend\",\"value\":{\"amount\":[{\"amount\":\"10000\",\"denom\":\"ift\"}],\"from_address\":\"cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j\",\"to_address\":\"cettest1hpgvshhx4t3mm0t54v5d7xu0qmw675nnqkhv8q\",\"unlock_time\":\"0\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AkvCoxJlFT8H5w4LqwhyTmuF4hf4zWKM62KXQke7STOC\"},\"signature\":\"QtNBJNVcog6d4SK7FHCxeiJtrCa9oOyhQtHYvVdmr1BOz+OhuwinqYWmfmoW91x+OMw9tZngeewROQu6Rp1lHQ==\"}],\"type\":\"bankx/MsgSend\"}}", output.json());

}

}
