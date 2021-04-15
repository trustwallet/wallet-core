// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Signer.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;

TEST(THORChainSigner, SignTx) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_memo("csut10husz");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r");
    message.set_to_address("thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("rune");
    amountOfTx->set_amount(50000000);

    auto &fee = *input.mutable_fee();
    fee.set_gas(2000000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("rune");
    amountOfFee->set_amount(200);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    EXPECT_EQ(R"({"fee":{"amounts":[{"denom":"rune","amount":"200"}],"gas":"2000000"},"memo":"csut10husz","messages":[{"sendCoinsMessage":{"fromAddress":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","toAddress":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn","amounts":[{"denom":"rune","amount":"50000000"}]}}]})", json);

    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Cosmos::Signer::sign(input);

    EXPECT_EQ(R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"rune"}],"gas":"2000000"},"memo":"csut10husz","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"50000000","denom":"rune"}],"from_address":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"a/QjYovh0KTQlfVm+lF8VrN4ofsku2DAYQnZaPDcsREMuW8dBxusSNwClna7PFB+x6WVIO+pYhrJ6PsxOJnphA=="}]}})", output.json());
    EXPECT_EQ(hex(output.signature()), "6bf423628be1d0a4d095f566fa517c56b378a1fb24bb60c06109d968f0dcb1110cb96f1d071bac48dc029676bb3c507ec7a59520efa9621ac9e8fb313899e984");
}

/*
TEST(THORChainSigner, Sign) {
    // TODO: Finalize test implementation

    //auto key = PrivateKey(parse_hex("__PRIVKEY_DATA__"));
    //auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    //auto from = Address(publicKey);
    //auto to = Address("__TO_ADDRESS__");
    //...
    //auto transaction = Transaction(...)
    //auto signature = Signer::sign(key, transaction);
    //auto result = transaction.serialize(signature);

    //ASSERT_EQ(hex(serialized), "__RESULT__");
    //ASSERT_EQ(...)
}
*/
