// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"
#include "TestUtilities.h"

#include <nlohmann/json.hpp>

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

namespace TW::Cosmos::evmos::tests {

TEST(EvmosSigner, SignTxJsonEthermintKeyType) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON); // obsolete
    input.set_account_number(1037);
    input.set_chain_id("evmos_9001-2");
    input.set_memo("");
    input.set_sequence(8);

    auto fromAddress = Address("evmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = Address("evmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("muon");
    amountOfTx->set_amount("1");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("200");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeNativeEvmos);
    auto anotherExpectedJson =R"(
                            {
                                "mode":"block",
                                "tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},
                                "memo":"",
                                "msg":[{"type":"cosmos-sdk/MsgSend",
                                "value":{"amount":[{"amount":"1","denom":"muon"}],
                                "from_address":"evmos1hsk6jryyqjfhp5dhc55tc9jtckygx0ep4mur4z",
                                "to_address":"evmos1zt50azupanqlfam5afhv3hexwyutnuke45f6ye"}}],
                                "signatures":
                                        [
                                            {
                                                "pub_key":
                                                        {
                                                            "type":"ethermint/PubKeyEthSecp256k1",
                                                            "value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
                                                        },
                                                "signature":"RWt8aaBxdMAeEjym8toWskJ6WaJpEF9Ciucz2lAHkvNnTicGpzxwTUzJbJXRirSnGkejhISaYtDw2RBiq0vg5w=="
                                            }
                                        ]}
                            })"_json;

    /// This tx is not broadcasted, we just want to test the signature format (ethermint/PubKeyEthSecp256k1)
    EXPECT_EQ(anotherExpectedJson, nlohmann::json::parse(output.json()));

    auto signatures = nlohmann::json::parse(output.signature_json());

    auto expectedSignatures = R"(
        [
            {
                "pub_key":
                    {
                        "type":"ethermint/PubKeyEthSecp256k1",
                        "value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
                    },
                "signature":"RWt8aaBxdMAeEjym8toWskJ6WaJpEF9Ciucz2lAHkvNnTicGpzxwTUzJbJXRirSnGkejhISaYtDw2RBiq0vg5w=="
            }
        ])"_json;
    EXPECT_EQ(signatures, expectedSignatures);
}

TEST(EvmosSigner, CompoundingAuthz) {
    // Successfully broadcasted https://www.mintscan.io/evmos/txs/8D811CEC078420C41220F0B584EA0AC037763380FAC31E0E352E4BB4D1D18B79
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(2139877);
    input.set_chain_id("evmos_9001-2");
    input.set_memo("");
    input.set_sequence(3);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_auth_grant();
    message.set_granter("evmos12m9grgas60yk0kult076vxnsrqz8xpjy9rpf3e");
    message.set_grantee("evmos18fzq4nac28gfma6gqfvkpwrgpm5ctar2z9mxf3");
    auto& grant_stake = *message.mutable_grant_stake();
    grant_stake.mutable_allow_list()->add_address("evmosvaloper1umk407eed7af6anvut6llg2zevnf0dn0feqqny");
    grant_stake.set_authorization_type(TW::Cosmos::Proto::Message_AuthorizationType_DELEGATE);
    message.set_expiration(1692309600);

    auto& fee = *input.mutable_fee();
    fee.set_gas(180859);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("aevmos");
    amountOfFee->set_amount("4521475000000000");

    auto privateKey = parse_hex("79bcbded1a5678ab34e6d9db9ad78e4e480e7b22723cc5fbf59e843732e1a8e5");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeNativeEvmos);
    auto expected = R"(
                {
                    "mode":"BROADCAST_MODE_BLOCK",
                    "tx_bytes":"CvUBCvIBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQSzwEKLGV2bW9zMTJtOWdyZ2FzNjB5azBrdWx0MDc2dnhuc3Jxejh4cGp5OXJwZjNlEixldm1vczE4ZnpxNG5hYzI4Z2ZtYTZncWZ2a3B3cmdwbTVjdGFyMno5bXhmMxpxCmcKKi9jb3Ntb3Muc3Rha2luZy52MWJldGExLlN0YWtlQXV0aG9yaXphdGlvbhI5EjUKM2V2bW9zdmFsb3BlcjF1bWs0MDdlZWQ3YWY2YW52dXQ2bGxnMnpldm5mMGRuMGZlcXFueSABEgYI4LD6pgYSfQpZCk8KKC9ldGhlcm1pbnQuY3J5cHRvLnYxLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohA4B2WHbj6sH/GWE7z/YW5PRnXYFGaGRAov7gZZI2Fv2nEgQKAggBGAMSIAoaCgZhZXZtb3MSEDQ1MjE0NzUwMDAwMDAwMDAQ+4QLGkAm17CZgB7m+CPVlITnrHosklMTL9zrUeGRs8FL8N0GcRami9zdJ+e3xuXOtJmwP7G6QNh85CRYjFj8a8lpmmJM"
                })";
    assertJSONEqual(output.serialized(), expected);
}
}
