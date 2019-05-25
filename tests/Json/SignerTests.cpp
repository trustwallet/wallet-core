// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>
#include <stdexcept>

#include "./Json/Signer.h"
#include "PrivateKey.h"
#include "Base64.h"

using namespace TW;
using namespace TW::Json;
using namespace google::protobuf::util;

TEST(JsonSigner, CosmosTransactionSign) {
    auto transaction = "{\"accountNumber\":\"8733\",\"chainId\":\"cosmoshub-2\",\"fee\":{\"amounts\":[{\"denom\":\"uatom\",\"amount\":\"5000\"}],\"gas\":\"200000\"},\"memo\":\"Testing\",\"sendCoinsMessage\":{\"fromAddress\":\"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575\",\"toAddress\":\"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0\",\"amounts\":[{\"denom\":\"uatom\",\"amount\":\"995000\"}]}}";
    auto privateKey = Base64::decode("ybCic4MZMapKX40aVw1QId2pHTMZvTgZvs2qv7e0Tjs=");
    auto input = Proto::SigningInput();
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_transaction(transaction);
    input.set_network(TWCoinTypeCosmos);

    auto signer = TW::Json::Signer(input);
    auto output = signer.sign();

    ASSERT_FALSE(output.has_error());
    ASSERT_EQ("", output.encoded());
    ASSERT_EQ("{\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"5000\",\"denom\":\"uatom\"}],\"gas\":\"200000\"},\"memo\":\"Testing\",\"msg\":[{\"type\":\"cosmos-sdk/MsgSend\",\"value\":{\"amount\":[{\"amount\":\"995000\",\"denom\":\"uatom\"}],\"from_address\":\"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575\",\"to_address\":\"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"A6EsukEXB53GhohQVeDpxtkeH8KQIayd/Co/ApYRYkTm\"},\"signature\":\"ULEpUqNzoAnYEx2x22F3ANAiPXquAU9+mqLWoAA/ZOUGTMsdb6vryzsW6AKX2Kqj1pGNdrTcQ58Z09JPyjpgEA==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}",
            output.json());
}

TEST(JsonSigner, NetworkNotSupported) {
    auto transaction = "{\"accountNumber\":\"8733\",\"chainId\":\"cosmoshub-2\",\"fee\":{\"amounts\":[{\"denom\":\"uatom\",\"amount\":\"5000\"}],\"gas\":\"200000\"},\"memo\":\"Testing\",\"sendCoinsMessage\":{\"fromAddress\":\"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575\",\"toAddress\":\"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0\",\"amounts\":[{\"denom\":\"uatom\",\"amount\":\"995000\"}]}}";
    auto privateKey = Base64::decode("ybCic4MZMapKX40aVw1QId2pHTMZvTgZvs2qv7e0Tjs=");
    auto input = Proto::SigningInput();
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_transaction(transaction);
    input.set_network(TWCoinTypeBitcoinCash);

    auto signer = TW::Json::Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.has_error());
    ASSERT_EQ(JsonSignErrorCodeNotSupported, output.error().code());
    ASSERT_EQ("Network not supported", output.error().description());
}

TEST(JsonSigner, InvalidJsonFormat) {
    auto transaction = "{\"accountNumber\":\"8733\"\"chainId\":\"cosmoshub-2\"}";
    auto privateKey = Base64::decode("ybCic4MZMapKX40aVw1QId2pHTMZvTgZvs2qv7e0Tjs=");
    auto input = Proto::SigningInput();
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_transaction(transaction);
    input.set_network(TWCoinTypeCosmos);

    auto signer = TW::Json::Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.has_error());
    ASSERT_EQ(JsonSignErrorCodeJsonParseError, output.error().code());
}