// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"

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

}
