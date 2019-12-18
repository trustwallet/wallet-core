// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Coinex.pb.h"
#include "Cosmos/Address.h"
#include "Coinex/Signer.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::Coinex {

TEST(CoinexAlias, SetAlias) {
    auto input = Proto::SigningInput();
    input.set_account_number(35);
    input.set_chain_id("coinexdex-test2001");
    input.set_memo("ifwallet");
    input.set_sequence(0);

    auto& message = *input.mutable_set_alias_message();
    message.set_owner("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
    message.set_alias("bibodeng");
    message.set_is_add(true);
    message.set_as_default(true);

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

    //ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"2000000\",\"denom\":\"cet\"}],\"gas\":\"100000\"},\"memo\":\"ifwallet\",\"msg\":[{\"type\":\"alias/MsgAliasUpdate\",\"value\":{\"alias\":\"bibodeng\",\"as_default\":true,\"is_add\":true,\"owner\":\"cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AkvCoxJlFT8H5w4LqwhyTmuF4hf4zWKM62KXQke7STOC\"},\"signature\":\"XTl0S8CTO2kwhJ7TPmGsCX+2u/sPMGF9SyLCyZ05XOU2KoIrRngYs4ud6HF/CPUEP+cUCxVSkUe56ROnEtsVVQ==\"}],\"type\":\"bankx/MsgSend\"}}", output.json());
    ASSERT_EQ("", output.json());

}

}
