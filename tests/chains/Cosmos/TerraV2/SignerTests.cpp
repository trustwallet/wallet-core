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
#include "Cosmos/ProtobufSerialization.h"
#include "uint256.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

namespace TW::Cosmos::tests {

TEST(TerraSigner, SignSendTx) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("phoenix-1");
    input.set_memo("");
    input.set_sequence(1);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("uluna");
    amountOfTx->set_amount("1000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("30000");

    {
        std::string json;
        google::protobuf::util::MessageToJsonString(input, &json);
        assertJSONEqual(json, R"(
            {
                "signingMode": "Protobuf",
                "accountNumber": "1037",
                "chainId": "phoenix-1",
                "fee": {
                    "amounts": [
                        {
                            "denom": "uluna",
                            "amount": "30000"
                        }
                    ],
                    "gas": "200000"
                },
                "sequence": "1",
                "messages": [
                    {
                        "sendCoinsMessage": {
                            "fromAddress": "terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2",
                            "toAddress": "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp",
                            "amounts": [
                                {
                                    "denom": "uluna",
                                    "amount": "1000000"
                                }
                            ]
                        }
                    }
                ]
            }
        )");
    }

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerraV2);

    // similar tx: https://finder.terra.money/mainnet/tx/fbbe73ad2f0db3a13911dc424f8a34370dc4b7e8b66687f536797e68ee200ece
    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "CpEBCo4BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm4KLHRlcnJhMWhzazZqcnl5cWpmaHA1ZGhjNTV0YzlqdGNreWd4MGVwMzdoZGQyEix0ZXJyYTFqbGdhcXk5bnZuMmhmNXQyc3JhOXljejhzNzd3bmY5bDBrbWdjcBoQCgV1bHVuYRIHMTAwMDAwMBJoClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECVyhuw/N9M1V7u6oACyd0SskCOqmWfK51oYHR/5H6ncUSBAoCCAEYARIUCg4KBXVsdW5hEgUzMDAwMBDAmgwaQPh0C3rjzdixIUiyPx3FlWAxzbKILNAcSRVeQnaTl1vsI5DEfYa2oYlUBLqyilcMCcU/iaJLhex30No2ak0Zn1Q=",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");
    EXPECT_EQ(hex(output.signature()), "f8740b7ae3cdd8b12148b23f1dc5956031cdb2882cd01c49155e427693975bec2390c47d86b6a1895404bab28a570c09c53f89a24b85ec77d0da366a4d199f54");
    EXPECT_EQ(output.error_message(), "");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraSigner, SignWasmTransferTx) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(3407705);
    input.set_chain_id("phoenix-1");
    input.set_memo("");
    input.set_sequence(3);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp", toAddress));
    const auto tokenContractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_execute_contract_transfer_message();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    const auto amount = store(uint256_t(250000), 0);
    message.set_amount(amount.data(), amount.size());
    message.set_recipient_address(toAddress.string());

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("3000");

    {
        std::string json;
        google::protobuf::util::MessageToJsonString(input, &json);
        assertJSONEqual(json, R"(
            {
                "signingMode": "Protobuf",
                "accountNumber": "3407705",
                "chainId": "phoenix-1",
                "fee": {
                    "amounts": [
                        {
                            "denom": "uluna",
                            "amount": "3000"
                        }
                    ],
                    "gas": "200000"
                },
                "sequence": "3",
                "messages": [
                    {
                        "wasmExecuteContractTransferMessage": {
                            "senderAddress": "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf",
                            "contractAddress": "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76",
                            "amount": "A9CQ",
                            "recipientAddress": "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"
                        }
                    }
                ]
            }
        )");
    }

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerraV2);

    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "CuUBCuIBCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QSuQEKLHRlcnJhMTh3dWtwODRkcTIyN3d1NG1naDBqbTZuOW5sbmo2cnM4MnBwOXdmEix0ZXJyYTE0ejU2bDBmcDJsc2Y4Nnp5M2h0eTJ6NDdlemtobnRodHI5eXE3NhpbeyJ0cmFuc2ZlciI6eyJhbW91bnQiOiIyNTAwMDAiLCJyZWNpcGllbnQiOiJ0ZXJyYTFqbGdhcXk5bnZuMmhmNXQyc3JhOXljejhzNzd3bmY5bDBrbWdjcCJ9fRJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYAxITCg0KBXVsdW5hEgQzMDAwEMCaDBpAiBGbQaj+jsXE6/FssD3fC77QOxpli9GqsPea+KoNyMIEgVj89Hii+oU1bAEQS4qV0SaE2V6RNy24uCcFTIRbcQ==",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");
    EXPECT_EQ(hex(output.signature()), "88119b41a8fe8ec5c4ebf16cb03ddf0bbed03b1a658bd1aab0f79af8aa0dc8c2048158fcf478a2fa85356c01104b8a95d12684d95e91372db8b827054c845b71");
    EXPECT_EQ(output.error_message(), "");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraSigner, SignWasmGeneric) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(3407705);
    input.set_chain_id("phoenix-1");
    input.set_memo("");
    input.set_sequence(7);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    const auto tokenContractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76";
    const auto txMessage = R"({"transfer": { "amount": "250000", "recipient": "terra1d7048csap4wzcv5zm7z6tdqem2agyp9647vdyj" } })";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_execute_contract_generic();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    message.set_execute_msg(txMessage);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("3000");

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerraV2);

    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "CuwBCukBCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QSwAEKLHRlcnJhMTh3dWtwODRkcTIyN3d1NG1naDBqbTZuOW5sbmo2cnM4MnBwOXdmEix0ZXJyYTE0ejU2bDBmcDJsc2Y4Nnp5M2h0eTJ6NDdlemtobnRodHI5eXE3NhpieyJ0cmFuc2ZlciI6IHsgImFtb3VudCI6ICIyNTAwMDAiLCAicmVjaXBpZW50IjogInRlcnJhMWQ3MDQ4Y3NhcDR3emN2NXptN3o2dGRxZW0yYWd5cDk2NDd2ZHlqIiB9IH0SZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awAEgQKAggBGAcSEwoNCgV1bHVuYRIEMzAwMBDAmgwaQGlYzOoAu/PfyCTSTisGJVW9KWwifxMbCmzy2xwqNg+ZHQkDjVRyUBl7gmbXXLzdOMqtwF1CMauJhlGwmEdzhK4=",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");

    EXPECT_EQ(hex(output.signature()), "6958ccea00bbf3dfc824d24e2b062555bd296c227f131b0a6cf2db1c2a360f991d09038d547250197b8266d75cbcdd38caadc05d4231ab898651b098477384ae");
    EXPECT_EQ(output.error_message(), "");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraSigner, SignWasmGenericWithCoins) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(3407705);
    input.set_chain_id("phoenix-1");
    input.set_memo("");
    input.set_sequence(9);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    const auto tokenContractAddress = "terra1sepfj7s0aeg5967uxnfk4thzlerrsktkpelm5s";
    const auto txMessage = R"({ "deposit_stable": {} })";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_execute_contract_generic();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    message.set_execute_msg(txMessage);

    auto amount = message.add_coins();
    amount->set_denom("uusd");
    amount->set_amount("1000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(600000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("7000");

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerraV2);

    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "CrABCq0BCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QShAEKLHRlcnJhMTh3dWtwODRkcTIyN3d1NG1naDBqbTZuOW5sbmo2cnM4MnBwOXdmEix0ZXJyYTFzZXBmajdzMGFlZzU5Njd1eG5mazR0aHpsZXJyc2t0a3BlbG01cxoYeyAiZGVwb3NpdF9zdGFibGUiOiB7fSB9KgwKBHV1c2QSBDEwMDASZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awAEgQKAggBGAkSEwoNCgV1bHVuYRIENzAwMBDAzyQaQEDA2foXegF+rslj6o8bX2HPJfn+q/6Ezbq2iAd0SFOTQqS8aAyywQkdZJRToXcaby1HOYL1WvmsMPgrFzChiY4=",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");

    EXPECT_EQ(hex(output.signature()), "40c0d9fa177a017eaec963ea8f1b5f61cf25f9feabfe84cdbab688077448539342a4bc680cb2c1091d649453a1771a6f2d473982f55af9ac30f82b1730a1898e");
    EXPECT_EQ(output.error_message(), "");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraSigner, SignWasmSendTx) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(3407705);
    input.set_chain_id("phoenix-1");
    input.set_memo("");
    input.set_sequence(4);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp", toAddress));
    const auto tokenContractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_execute_contract_send_message();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    const auto amount = store(uint256_t(250000), 0);
    message.set_amount(amount.data(), amount.size());
    message.set_recipient_contract_address(toAddress.string());
    const auto msgMsg = Base64::encode(data(std::string(R"({"some_message":{}})")));
    EXPECT_EQ(msgMsg, "eyJzb21lX21lc3NhZ2UiOnt9fQ==");
    message.set_msg(msgMsg);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("3000");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);
    assertJSONEqual(json, R"(
        {
            "signingMode": "Protobuf",
            "accountNumber": "3407705",
            "chainId": "phoenix-1",
            "fee": {
                "amounts": [
                    {
                        "denom": "uluna",
                        "amount": "3000"
                    }
                ],
                "gas": "200000"
            },
            "sequence": "4",
            "messages": [
                {
                    "wasmExecuteContractSendMessage": {
                        "senderAddress": "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf",
                        "contractAddress": "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76",
                        "amount": "A9CQ",
                        "recipientContractAddress": "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp",
                        "msg": "eyJzb21lX21lc3NhZ2UiOnt9fQ=="
                    }
                }
            ]
        }
    )");

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerraV2);

    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "CoUCCoICCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QS2QEKLHRlcnJhMTh3dWtwODRkcTIyN3d1NG1naDBqbTZuOW5sbmo2cnM4MnBwOXdmEix0ZXJyYTE0ejU2bDBmcDJsc2Y4Nnp5M2h0eTJ6NDdlemtobnRodHI5eXE3Nhp7eyJzZW5kIjp7ImFtb3VudCI6IjI1MDAwMCIsImNvbnRyYWN0IjoidGVycmExamxnYXF5OW52bjJoZjV0MnNyYTl5Y3o4czc3d25mOWwwa21nY3AiLCJtc2ciOiJleUp6YjIxbFgyMWxjM05oWjJVaU9udDlmUT09In19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgEEhMKDQoFdWx1bmESBDMwMDAQwJoMGkBKJbW1GDrv9j2FIckm7MtpDZzP2RjgDjU84oYmOHNHsxEBPLjtt3YAjsKWBCAsjbnbVoJ3s2XFG08nxQXS9xBK",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");
    EXPECT_EQ(hex(output.signature()), "4a25b5b5183aeff63d8521c926eccb690d9ccfd918e00e353ce28626387347b311013cb8edb776008ec29604202c8db9db568277b365c51b4f27c505d2f7104a");
    EXPECT_EQ(output.error_message(), "");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraSigner, SignWasmTransferPayload) {
    auto proto = Proto::Message_WasmExecuteContractTransfer();
    proto.set_recipient_address("recipient=address");
    const auto amount = store(uint256_t(250000), 0);
    proto.set_amount(amount.data(), amount.size());

    const auto payload = Protobuf::wasmExecuteTransferPayload(proto);

    assertJSONEqual(payload.dump(), R"(
        {
            "transfer":
                {
                    "amount": "250000",
                    "recipient": "recipient=address"
                }
        }
    )");
}

} // namespace TW::Cosmos::tests
