// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;


TEST(TerraSigner, SignSendTx) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON);
    input.set_account_number(1037);
    input.set_chain_id("columbus-5");
    input.set_memo("");
    input.set_sequence(2);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("luna");
    amountOfTx->set_amount(1000000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("luna");
    amountOfFee->set_amount(200);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);
    EXPECT_EQ(json, R"({"accountNumber":"1037","chainId":"columbus-5","fee":{"amounts":[{"denom":"luna","amount":"200"}],"gas":"200000"},"sequence":"2","messages":[{"sendCoinsMessage":{"fromAddress":"terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2","toAddress":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf","amounts":[{"denom":"luna","amount":"1000000"}]}}]})");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    assertJSONEqual(output.json(), R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"luna"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1000000","denom":"luna"}],"from_address":"terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2","to_address":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"ofdIsLJzkODcQwLG89eE2g4HOaUmfKPh/08t07ehKPUqRMl4rVonzo73mkOvqtrHWjdtB+6t6R8DGudPpb6bRg=="}]}})");
    EXPECT_EQ(hex(output.signature()), "a1f748b0b27390e0dc4302c6f3d784da0e0739a5267ca3e1ff4f2dd3b7a128f52a44c978ad5a27ce8ef79a43afaadac75a376d07eeade91f031ae74fa5be9b46");
    EXPECT_EQ(output.serialized(), "");
    EXPECT_EQ(output.error(), "");
}

TEST(TerraSigner, SignWasmTransferTx) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON);
    input.set_account_number(1037);
    input.set_chain_id("columbus-5");
    input.set_memo("");
    input.set_sequence(2);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", toAddress));
    const auto tokenContractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76"; // ANC

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_execute_contract_transfer_message();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    message.set_amount(1000000);
    message.set_recipient_address(toAddress.string());

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("luna");
    amountOfFee->set_amount(200);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);
    EXPECT_EQ(json, R"({"accountNumber":"1037","chainId":"columbus-5","fee":{"amounts":[{"denom":"luna","amount":"200"}],"gas":"200000"},"sequence":"2","messages":[{"wasmExecuteContractTransferMessage":{"senderAddress":"terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2","contractAddress":"terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76","amount":"1000000","recipientAddress":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}]})");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    assertJSONEqual(output.json(), R"(
        {
            "mode":"block",
            "tx":
                {
                    "fee": {"amount":[{"amount":"200","denom":"luna"}],"gas":"200000"},
                    "memo":"",
                    "msg":
                        [
                            {
                                "type":"wasm/MsgExecuteContract",
                                "value":
                                    {
                                        "sender": "terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2",
                                        "contract": "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76",
                                        "execute_msg": "eyJ0cmFuc2ZlciI6eyJhbW91bnQiOiIxMDAwMDAwIiwicmVjaXBpZW50IjoidGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YifX0=",
                                        "coins": []
                                    }
                            }
                        ],
                    "signatures":
                        [
                            {
                                "pub_key":
                                    {
                                        "type": "tendermint/PubKeySecp256k1",
                                        "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
                                    },
                                "signature": "Vi6/1XljuueyJw6L8a8KHUNMhqosbP0SuRpySWNHQfh8HqUg459Ax13S8kOEi5/POy++RF3UDSmmnz1LmgH2lA=="
                            }
                        ]
                }
        })");
    EXPECT_EQ(hex(output.signature()), "562ebfd57963bae7b2270e8bf1af0a1d434c86aa2c6cfd12b91a7249634741f87c1ea520e39f40c75dd2f243848b9fcf3b2fbe445dd40d29a69f3d4b9a01f694");
    EXPECT_EQ(output.serialized(), "");
    EXPECT_EQ(output.error(), "");
}
