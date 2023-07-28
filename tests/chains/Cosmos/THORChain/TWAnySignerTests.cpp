// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include "Cosmos/Address.h"
#include "proto/Cosmos.pb.h"
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(THORChainTWAnySigner, SignTx) {
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
    Cosmos::Proto::SigningInput input;
    input.set_account_number(593);
    input.set_chain_id("thorchain");
    input.set_sequence(3);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_memo("");

    auto fromAddress = "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r";
    auto toAddress = "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress);
    message.set_to_address(toAddress);
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("rune");
    amountOfTx->set_amount("10000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("rune");
    amountOfFee->set_amount("2000000");

    Cosmos::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTHORChain);

    // https://viewblock.io/thorchain/tx/FD0445AFFC4ED9ACCB7B5D3ADE361DAE4596EA096340F1360F1020381EA454AF
    ASSERT_EQ(output.json(), R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"2000000","denom":"rune"}],"gas":"200000"},"memo":"","msg":[{"type":"thorchain/MsgSend","value":{"amount":[{"amount":"10000000","denom":"rune"}],"from_address":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"qgpMX3WNq4DsNBnYtdmBD4ejiailK4uI/m3/YVqCSNF8AtkUOTmP48ztqCbpkWTFvw1/9S8/ivsFxOcK6AI0jA=="}]}})");
}

TEST(THORChainTWAnySigner, SignTx_Maya) {
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
    Cosmos::Proto::SigningInput input;
    input.set_account_number(593);
    input.set_chain_id("mayachain");
    input.set_sequence(3);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_memo("");

    auto fromAddress = "maya1xhfwp27s3y3lsnzp6d0qd870q9vleelunq7f2f";
    auto toAddress = "maya1xav46urwavk3t4c23wwfvvk2kusxxlpr2k7meq";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress);
    message.set_to_address(toAddress);
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("cacao");
    amountOfTx->set_amount("100000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cacao");
    amountOfFee->set_amount("4000000");

    Cosmos::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeMAYAChain);

    // https://www.explorer.mayachain.info/tx/523C1BEDAC87E8528EC5659BE4CC952CE762C0F7D320C7E26A1F09B25863B06A/
    ASSERT_EQ(output.json(), R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"4000000","denom":"cacao"}],"gas":"200000"},"memo":"","msg":[{"type":"mayachain/MsgSend","value":{"amount":[{"amount":"100000000","denom":"cacao"}],"from_address":"maya1xhfwp27s3y3lsnzp6d0qd870q9vleelunq7f2f","to_address":"maya1xav46urwavk3t4c23wwfvvk2kusxxlpr2k7meq"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"UYTlt/4rN2JEK3ByOgyQjyW0II6KgtT+QdQRXdhHB6Nfy5g19Tv09Zl6Ip5bTKe6rBKZSUVP1G+xARcQLfS/Gw=="}]}})");
}