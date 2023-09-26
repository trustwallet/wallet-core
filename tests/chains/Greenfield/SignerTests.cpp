// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Greenfield/Signer.h"
#include "Greenfield/SignerEip712.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

namespace TW::Greenfield::tests {

TEST(GreenfieldSigner, SignerEip712Send) {
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15560);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(2);

    auto& msg = *input.add_messages();
    auto& msgSend = *msg.mutable_send_coins_message();
    msgSend.set_from_address("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
    msgSend.set_to_address("0x280b27f3676db1C4475EE10F75D510Eb527fd155");
    auto amountOfTx = msgSend.add_amounts();
    amountOfTx->set_denom("BNB");
    amountOfTx->set_amount("1000000000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("2000000000000000");

    auto typedData = SignerEip712::wrapTxToTypedData(input).payload();
    auto expectedJson = json::parse(R"(
{
    "types": {
        "Coin": [
            {
                "name": "amount",
                "type": "uint256"
            },
            {
                "name": "denom",
                "type": "string"
            }
        ],
        "EIP712Domain": [
            {
                "name": "chainId",
                "type": "uint256"
            },
            {
                "name": "name",
                "type": "string"
            },
            {
                "name": "salt",
                "type": "string"
            },
            {
                "name": "verifyingContract",
                "type": "string"
            },
            {
                "name": "version",
                "type": "string"
            }
        ],
        "Fee": [
            {
                "name": "amount",
                "type": "Coin[]"
            },
            {
                "name": "gas_limit",
                "type": "uint256"
            },
            {
                "name": "granter",
                "type": "string"
            },
            {
                "name": "payer",
                "type": "string"
            }
        ],
        "Msg1": [
            {
                "name": "amount",
                "type": "TypeMsg1Amount[]"
            },
            {
                "name": "from_address",
                "type": "string"
            },
            {
                "name": "to_address",
                "type": "string"
            },
            {
                "name": "type",
                "type": "string"
            }
        ],
        "Tx": [
            {
                "name": "account_number",
                "type": "uint256"
            },
            {
                "name": "chain_id",
                "type": "uint256"
            },
            {
                "name": "fee",
                "type": "Fee"
            },
            {
                "name": "memo",
                "type": "string"
            },
            {
                "name": "msg1",
                "type": "Msg1"
            },
            {
                "name": "sequence",
                "type": "uint256"
            },
            {
                "name": "timeout_height",
                "type": "uint256"
            }
        ],
        "TypeMsg1Amount": [
            {
                "name": "amount",
                "type": "string"
            },
            {
                "name": "denom",
                "type": "string"
            }
        ]
    },
    "primaryType": "Tx",
    "domain": {
        "name": "Greenfield Tx",
        "version": "1.0.0",
        "chainId": "5600",
        "verifyingContract": "greenfield",
        "salt": "0"
    },
    "message": {
        "account_number": "15560",
        "chain_id": "5600",
        "fee": {
            "amount": [
                {
                    "amount": "2000000000000000",
                    "denom": "BNB"
                }
            ],
            "gas_limit": "200000",
            "granter": "",
            "payer": "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1"
        },
        "memo": "",
        "msg1": {
            "amount": [
                {
                    "amount": "1000000000000000",
                    "denom": "BNB"
                }
            ],
            "from_address": "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1",
            "to_address": "0x280b27f3676db1C4475EE10F75D510Eb527fd155",
            "type": "/cosmos.bank.v1beta1.MsgSend"
        },
        "sequence": "2",
        "timeout_height": "0"
    }
}
    )");
    EXPECT_EQ(typedData, expectedJson);

    auto expectedPreHash = "b8c62654582ca96b37ca94966199682bf70ed934e740d2f874ff54675a0ac344";
    auto preHash = SignerEip712::preImageHash(input).payload();
    EXPECT_EQ(hex(preHash.typedDataHash), expectedPreHash);

    auto privateKey = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signature = SignerEip712::sign(input).payload();
    auto expectedSignature = "cb3a4684a991014a387a04a85b59227ebb79567c2025addcb296b4ca856e9f810d3b526f2a0d0fad6ad1b126b3b9516f8b3be020a7cca9c03ce3cf47f4199b6d1b";
    EXPECT_EQ(hex(signature), expectedSignature);
}

TEST(GreenfieldSigner, SignerEip712TransferOut) {
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15560);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(2);

    auto& msg = *input.add_messages();
    auto& msgSend = *msg.mutable_bridge_transfer_out();
    msgSend.set_from_address("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
    msgSend.set_to_address("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
    msgSend.mutable_amount()->set_denom("BNB");
    msgSend.mutable_amount()->set_amount("1000000000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("2000000000000000");

    auto typedData = SignerEip712::wrapTxToTypedData(input).payload();
    auto expectedJson = json::parse(R"(
{
    "types": {
        "Coin": [
            {
                "name": "amount",
                "type": "uint256"
            },
            {
                "name": "denom",
                "type": "string"
            }
        ],
        "EIP712Domain": [
            {
                "name": "chainId",
                "type": "uint256"
            },
            {
                "name": "name",
                "type": "string"
            },
            {
                "name": "salt",
                "type": "string"
            },
            {
                "name": "verifyingContract",
                "type": "string"
            },
            {
                "name": "version",
                "type": "string"
            }
        ],
        "Fee": [
            {
                "name": "amount",
                "type": "Coin[]"
            },
            {
                "name": "gas_limit",
                "type": "uint256"
            },
            {
                "name": "granter",
                "type": "string"
            },
            {
                "name": "payer",
                "type": "string"
            }
        ],
        "Msg1": [
            {
                "name": "amount",
                "type": "TypeMsg1Amount"
            },
            {
                "name": "from",
                "type": "string"
            },
            {
                "name": "to",
                "type": "string"
            },
            {
                "name": "type",
                "type": "string"
            }
        ],
        "Tx": [
            {
                "name": "account_number",
                "type": "uint256"
            },
            {
                "name": "chain_id",
                "type": "uint256"
            },
            {
                "name": "fee",
                "type": "Fee"
            },
            {
                "name": "memo",
                "type": "string"
            },
            {
                "name": "msg1",
                "type": "Msg1"
            },
            {
                "name": "sequence",
                "type": "uint256"
            },
            {
                "name": "timeout_height",
                "type": "uint256"
            }
        ],
        "TypeMsg1Amount": [
            {
                "name": "amount",
                "type": "string"
            },
            {
                "name": "denom",
                "type": "string"
            }
        ]
    },
    "primaryType": "Tx",
    "domain": {
        "name": "Greenfield Tx",
        "version": "1.0.0",
        "chainId": "5600",
        "verifyingContract": "greenfield",
        "salt": "0"
    },
    "message": {
        "account_number": "15560",
        "chain_id": "5600",
        "fee": {
            "amount": [
                {
                    "amount": "2000000000000000",
                    "denom": "BNB"
                }
            ],
            "gas_limit": "200000",
            "granter": "",
            "payer": "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1"
        },
        "memo": "",
        "msg1": {
            "amount": {
                "amount": "1000000000000000",
                "denom": "BNB"
            },
            "from": "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1",
            "to": "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1",
            "type": "/greenfield.bridge.MsgTransferOut"
        },
        "sequence": "2",
        "timeout_height": "0"
    }
}
    )");
    EXPECT_EQ(typedData, expectedJson);

    auto expectedPreHash = "ea7731461041f5f652ab424bb767c670e484cfe1f4a85179deba8a6596873af4";
    auto preHash = SignerEip712::preImageHash(input).payload();
    EXPECT_EQ(hex(preHash.typedDataHash), expectedPreHash);

    auto privateKey = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signature = SignerEip712::sign(input).payload();
    auto expectedSignature = "c345fe0deb4fd93da5e808f6bd8aac3fb9de70fea2774e4657c37b02143135e37a02d53e8696edaede4a3e2e624eebd3261f43e02972812c11b356e236c834141c";
    EXPECT_EQ(hex(signature), expectedSignature);
}

TEST(GreenfieldSigner, SignMsgSend9F895C) {
    // Successfully broadcasted https://greenfieldscan.com/tx/0x9f895cf2dd64fb1f428cefcf2a6585a813c3540fc9fe1ef42db1da2cb1df55ab

    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15560);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(2);
    input.set_mode(Proto::BroadcastMode::SYNC);

    auto& msg = *input.add_messages();
    auto& msgSend = *msg.mutable_send_coins_message();
    msgSend.set_from_address("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
    msgSend.set_to_address("0x280b27f3676db1C4475EE10F75D510Eb527fd155");
    auto amountOfTx = msgSend.add_amounts();
    amountOfTx->set_denom("BNB");
    amountOfTx->set_amount("1000000000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("2000000000000000");

    auto privateKey = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(hex(output.signature()), "cb3a4684a991014a387a04a85b59227ebb79567c2025addcb296b4ca856e9f810d3b526f2a0d0fad6ad1b126b3b9516f8b3be020a7cca9c03ce3cf47f4199b6d1b");
    EXPECT_EQ(output.serialized(), R"({"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CpQBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4OWQxZDk3YURGY2QzMjRCYmQ2MDNEMzg3MkJENzhlMDQwOTg1MTBiMRIqMHgyODBiMjdmMzY3NmRiMUM0NDc1RUUxMEY3NUQ1MTBFYjUyN2ZkMTU1GhcKA0JOQhIQMTAwMDAwMDAwMDAwMDAwMBJ5ClgKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAnnvNAZNoQ2wRjxwSAYWugIHA+w6RQJt73vr0ggvXW/IEgUKAwjIBRgCEh0KFwoDQk5CEhAyMDAwMDAwMDAwMDAwMDAwEMCaDBpByzpGhKmRAUo4egSoW1kifrt5VnwgJa3cspa0yoVun4ENO1JvKg0PrWrRsSazuVFvizvgIKfMqcA8489H9BmbbRs="})");
}

TEST(GreenfieldSigner, SignMsgSendB798AB) {
    // Successfully broadcasted https://greenfieldscan.com/tx/B798AB548B74B9B410F9464CA2B29C6AFEC3B4F45050338FC34F9DFC057C4D2A

    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15560);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(3);
    input.set_mode(Proto::BroadcastMode::SYNC);

    auto& msg = *input.add_messages();
    auto& msgSend = *msg.mutable_send_coins_message();
    msgSend.set_from_address("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
    msgSend.set_to_address("0x280b27f3676db1C4475EE10F75D510Eb527fd155");
    auto amountOfTx = msgSend.add_amounts();
    amountOfTx->set_denom("BNB");
    amountOfTx->set_amount("5000000000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(1200);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("6000000000000");

    auto privateKey = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(hex(output.signature()), "37bc208e75cb16417f53bec4e6092f42da95aca22413ed4da9af41b64fd59fec0b35da17e54a31ceeda083e1185ef23df8d9805e8886b9b892fce570587b5a951b");
    EXPECT_EQ(output.serialized(), R"({"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CpQBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4OWQxZDk3YURGY2QzMjRCYmQ2MDNEMzg3MkJENzhlMDQwOTg1MTBiMRIqMHgyODBiMjdmMzY3NmRiMUM0NDc1RUUxMEY3NUQ1MTBFYjUyN2ZkMTU1GhcKA0JOQhIQNTAwMDAwMDAwMDAwMDAwMBJ1ClgKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAnnvNAZNoQ2wRjxwSAYWugIHA+w6RQJt73vr0ggvXW/IEgUKAwjIBRgDEhkKFAoDQk5CEg02MDAwMDAwMDAwMDAwELAJGkE3vCCOdcsWQX9TvsTmCS9C2pWsoiQT7U2pr0G2T9Wf7As12hflSjHO7aCD4Rhe8j342YBeiIa5uJL85XBYe1qVGw=="})");
}

TEST(GreenfieldSigner, SignMsgTransferOut) {
    // Successfully broadcasted Greenfield: https://greenfieldscan.com/tx/38C29C530A74946CFD22EE07DA642F5EF9399BC9AEA59EC56A9B5BE16DE16CE7
    // BSC (parent transaction): https://testnet.bscscan.com/tx/0x7f73c8a362e14e58cb5e0ec17616afc50eff7aa398db472383a6d017c8a5861a

    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15560);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(7);
    input.set_mode(Proto::BroadcastMode::SYNC);

    auto& msg = *input.add_messages();
    auto& msgTransferOut = *msg.mutable_bridge_transfer_out();
    msgTransferOut.set_from_address("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
    msgTransferOut.set_to_address("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
    auto amountOfTx = msgTransferOut.mutable_amount();
    amountOfTx->set_denom("BNB");
    amountOfTx->set_amount("5670000000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(1200);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("6000000000000");

    auto privateKey = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(hex(output.signature()), "ce030c1ca1bbcd14cbf337b1078fbc09a199b15665e1aadf9f6bba0b6c90489729f0b3481f3a5e6062fa5f90e444c34b67e894cc5bb9f1d213c80ce160421f381c");
    EXPECT_EQ(output.serialized(), R"({"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CpkBCpYBCiEvZ3JlZW5maWVsZC5icmlkZ2UuTXNnVHJhbnNmZXJPdXQScQoqMHg5ZDFkOTdhREZjZDMyNEJiZDYwM0QzODcyQkQ3OGUwNDA5ODUxMGIxEioweDlkMWQ5N2FERmNkMzI0QmJkNjAzRDM4NzJCRDc4ZTA0MDk4NTEwYjEaFwoDQk5CEhA1NjcwMDAwMDAwMDAwMDAwEnUKWApNCiYvY29zbW9zLmNyeXB0by5ldGguZXRoc2VjcDI1NmsxLlB1YktleRIjCiECee80Bk2hDbBGPHBIBha6AgcD7DpFAm3ve+vSCC9db8gSBQoDCMgFGAcSGQoUCgNCTkISDTYwMDAwMDAwMDAwMDAQsAkaQc4DDByhu80Uy/M3sQePvAmhmbFWZeGq359rugtskEiXKfCzSB86XmBi+l+Q5ETDS2folMxbufHSE8gM4WBCHzgc"})");
}

TEST(GreenfieldSigner, SignNoMessages) {
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15560);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(3);
    input.set_mode(Proto::BroadcastMode::SYNC);

    auto& fee = *input.mutable_fee();
    fee.set_gas(1200);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("6000000000000");

    auto privateKey = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    EXPECT_EQ(output.error(), Common::Proto::SigningError::Error_invalid_params);
}

TEST(GreenfieldSigner, SignMultipleMessages) {
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15560);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(3);
    input.set_mode(Proto::BroadcastMode::SYNC);

    // Append two empty messages.
    input.add_messages();
    input.add_messages();

    auto& fee = *input.mutable_fee();
    fee.set_gas(1200);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("6000000000000");

    auto privateKey = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    EXPECT_EQ(output.error(), Common::Proto::SigningError::Error_invalid_params);
}

} // namespace TW::Greenfield::tests
