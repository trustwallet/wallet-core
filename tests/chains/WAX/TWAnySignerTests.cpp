// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include "EOS/Address.h"
#include "Base58.h"
#include "proto/EOS.pb.h"

#include <gtest/gtest.h>

namespace TW::EOS::tests {

TEST(TWAnySignerWAX, Sign) {
    Proto::SigningInput input;
    const auto chainId = parse_hex("1064487b3cd1a897ce03ae5b6a865651747e2e152090f99c1d19d44e01aea5a4");
    const auto refBlock = parse_hex("0cfccda69fd86cd47dba49421a25d47d2a70a718e3cd597f6728a09d091316f6");
    const auto key = parse_hex("<redacted>");

    const auto pubKey = PublicKey(PrivateKey(key).getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(pubKey);
    EXPECT_EQ(address.string(), "EOS7rC6zYUjuxWkiokZTrwwHqwFvZ15Qdrn5WNxMKVXtHiDDmBWog");

    auto& asset = *input.mutable_asset();
    asset.set_amount(100000000);
    asset.set_decimals(8);
    asset.set_symbol("WAX");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_reference_block_id(refBlock.data(), refBlock.size());
    input.set_reference_block_time(1670413402);
    input.set_currency("eosio.token");
    input.set_sender("k52o1qdeh.gm");
    input.set_recipient("c2lrpvzxb.gm");
    input.set_memo("sent from wallet-core");
    input.set_private_key(key.data(), key.size());
    input.set_private_key_type(Proto::KeyType::MODERNK1);

    {
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEOS);

        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(output.json_encoded(), R"({\"compression\":\"none\",\"packed_context_free_data\":\"\",\"packed_trx\":\"787c9063a6cd7dba4942000000000100a6823403ea3055000000572d3ccdcd012019682ad940458100000000a8ed3232362019682ad9404581201938fdef7aa34000e1f5050000000008574158000000001573656e742066726f6d2077616c6c65742d636f726500\",\"signatures":["SIG_K1_KjAotwbF7RCWUpgS1WBpShfVAo2ffwm5BSeUNBT7HPGghPZZ6K3ovf86UrRSTv6oR6r5u2YE61uyd1MxcqxyxrkqZdGktG"]})");
    }

    input.set_private_key_type(Proto::KeyType::LEGACY);
    {
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEOS);
        EXPECT_EQ(output.error(), Common::Proto::Error_internal);
        EXPECT_TRUE(output.json_encoded().empty());
    }
}

} // namespace TW::EOS::tests
