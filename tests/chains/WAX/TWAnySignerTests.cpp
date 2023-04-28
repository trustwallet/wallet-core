// Copyright © 2017-2023 Trust Wallet.
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
    const auto refBlock = parse_hex("0cffaeda15039f3468398c5b4295d220fcc217f7cf96030c3729773097c6bd76");
    const auto key = parse_hex("d30d185a296b9591d648cb92fe0aa8f8a42de30ed9d2a21da9e7f69c67e8e355");

    const auto pubKey = PublicKey(PrivateKey(key).getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(pubKey);
    EXPECT_EQ(address.string(), "EOS7rC6zYUjuxWkiokZTrwwHqwFvZ15Qdrn5WNxMKVXtHiDDmBWog");

    auto& asset = *input.mutable_asset();
    asset.set_amount(100000000);
    asset.set_decimals(4);
    asset.set_symbol("WAX");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_reference_block_id(refBlock.data(), refBlock.size());
    input.set_reference_block_time(1670507804);
    input.set_currency("eosio.token");
    input.set_sender("k52o1qdeh.gm");
    input.set_recipient("c2lrpvzxb.gm");
    input.set_memo("sent from wallet-core");
    input.set_private_key(key.data(), key.size());
    input.set_private_key_type(Proto::KeyType::MODERNK1);
    input.set_expiration(1670507804 + 30);

    // https://wax.bloks.io/transaction/4548f7b28ee608663caea61234049ac0018415e02dd0abcea1c215c8da00d10a
    {
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEOS);

        EXPECT_EQ(output.error(), Common::Proto::OK);
        auto expected = R"({"compression":"none","packed_context_free_data":"","packed_trx":"3aed9163daae68398c5b000000000100a6823403ea3055000000572d3ccdcd012019682ad940458100000000a8ed3232362019682ad9404581201938fdef7aa34000e1f5050000000004574158000000001573656e742066726f6d2077616c6c65742d636f726500","signatures":["SIG_K1_KAroa9t89dpujjfBgBMgDcZrVhML5yP7iFk5sGNnNqbT4SxTCLqjQwwLZDi1ryx4W7Hy9DE9p1MqUSFVKeY8NtKyiySFjE"]})";
        EXPECT_EQ(output.json_encoded(), expected);
    }
}

} // namespace TW::EOS::tests
