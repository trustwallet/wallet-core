// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/AddressV3.h"
#include "Cardano/Signer.h"
#include "proto/Cardano.pb.h"
#include <TrustWalletCore/TWCardano.h>

#include "Cbor.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "uint256.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace std;

namespace TW::Cardano::tests {

const auto privateKeyTest1 = "089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e";
const auto ownAddress1 = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23";
const auto stakingKey1 = "df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b";
const auto poolIdNufi = "7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6";
//const auto poolIdEmur3 = "c34a7f59c556633dc88ec25c9743c5ebca3705e179a54db5638941cb";

TEST(CardanoStaking, Delegate_and_Stake_real_CT2) {
    const auto privateKeyData = parse_hex("20f63a2961b3b829c75a2efe067467bbbaab91790f0e04b8421fca21355fb64352c0bb10f084126323d72753a32f6eca84057ca290eb2d947f0d98c30663f0e9c6bb5d7a27bed7b6323acdbc7343a59820e9260d089f533e603653d7c502cb0208aa67a101e6e0d100b22d247c05a7b92732d73f76deca4a90185af6335fb643c1336e7f19ef030c641bf0bbe6e4a560c7876f6c83e8e38e8deb6dbf2fcda5ed8462d2123181dd712e3fa359f116b04dfbd52c0ebff8ab93354f45f10b6eca45");
    //const auto privateKeyDataTA2 = parse_hex("484d5bc3375435c9a0b0d121e86d6eba7d55929a263530d75bb1cb1d098bc653f75e5127219208a9d28ca6fecd95530b97470957f72be0648b2c99e2a29cc841fa49248845144b2dbccf6e85c172cd44099f5977e7126209e7c3e4b1fba4a264f0607785e71ace08560b30e3ffaab13f46418493f223e1407c0e6358108bc65351b30406485ad396ac0a390f6d718413fc5256069a36dd7bc2670fc49597d83556a8d4c88d10db7883dad1e70e03bbfbdff3545d33eb4cf2ea8f044a29462cde");
    const auto publicKey = PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, "addr1qy96ssc85r70c7j8m34hg73tetc2jmjrwwux7yxr7qvky0aeh3ed6q6p4mv5kahq73tvy2wwwlgvjmy3m97k5n67y0fs5ey8cj");
    const auto stakingKeyHash = subData(AddressV3(publicKey).bytes, 28, 28); // TODO obtain from API
    EXPECT_EQ(hex(stakingKeyHash), "b9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d3");
    const auto poolId = parse_hex(poolIdNufi);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(4000000ul);
    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(1);
    utxo2->set_address(ownAddress);
    utxo2->set_amount(26651312ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_amount(4000000ul);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(69885081ul);

    // Register staking key, 2 ADA deposit
    input.mutable_register_staking_key()->set_staking_key(stakingKeyHash.data(), stakingKeyHash.size());
    input.mutable_register_staking_key()->set_deposit_amount(2000000ul);

    // Delegate
    input.mutable_delegate()->set_staking_key(stakingKeyHash.data(), stakingKeyHash.size());
    input.mutable_delegate()->set_pool_id(poolId.data(), poolId.size());
    input.mutable_delegate()->set_deposit_amount(0ul);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        const auto amount = 28475125ul;
        EXPECT_EQ(plan.availableAmount, 30651312ul);
        EXPECT_EQ(plan.amount, amount);
        EXPECT_EQ(plan.fee, 176187ul);
        EXPECT_EQ(plan.change, 30651312ul - 2000000ul - amount - 176187ul);
        //ASSERT_EQ(plan.utxos.size(), 1ul);
        //EXPECT_EQ(plan.utxos[0].amount, 28651312ul);

        // perform sign with default plan
        const auto output = signer.sign();

        EXPECT_EQ(output.error(), Common::Proto::OK);

        const auto encoded = data(output.encoded());
        EXPECT_EQ(hex(encoded), "83a500828258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e008258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e010181825839010ba84307a0fcfc7a47dc6b747a2bcaf0a96e4373b86f10c3f019623fb9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d31a01b27ef5021a0002b03b031a042a5c99048282008200581cb9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d383028200581cb9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d3581c7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6a1008282582065d6f4da4c7e6354664477078468751d5844603e0298d87b210a4a1b3084ff985840e3adb41edefc6ebca4128de056eb9932e503f7dd3418b9e69ce32cbbbfec095b53a75636f07db6a5dbf0a2d5d12e7ad2266987ee6b199774d77fe3c2a570400b82582092ca29f2ec2f81df5c2256f6715b35f9db0f2c9cde616d440ffbad420bfc75b05840cffa2fa3f7927ee09695f1f03bd73fbe3445e38ddeb40315cc447c7c30092126019a048b0e2c14be57937e9654c2638c5bc4e0f99c45e4c156dd48c6ed151200f6");
        const auto txid = data(output.tx_id());
        EXPECT_EQ(hex(txid), "ea6dcde52c6ea76ef47d315429cfc3a98e149ea52d4cb929d1a1323f679623c2");
    }

    // set different plan, with one input only
    const auto amount = 28467322ul;
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(28651312ul);
    input.mutable_plan()->set_fee(183990);
    input.mutable_plan()->set_change(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(1);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a500828258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e008258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e010181825839010ba84307a0fcfc7a47dc6b747a2bcaf0a96e4373b86f10c3f019623fb9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d31a01b2607a021a0002ceb6031a042a5c99048282008200581cb9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d383028200581cb9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d3581c7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6a1008282582065d6f4da4c7e6354664477078468751d5844603e0298d87b210a4a1b3084ff985840f2b392094a5bcc0663ddc26ce74fe2aa942fb9448ed46cf3ce3dfc08b23bf2414a136fd52956a564d8d19f8d613063969bc7169d7cfe41dd1219babab83eae0582582092ca29f2ec2f81df5c2256f6715b35f9db0f2c9cde616d440ffbad420bfc75b0584085d5908503e17d69ea27809081e530a2f9c64e405469bb251648695b0626affe06ceb1d36b439675d38e46362f12e403adcecc4f94416a830de923672cb75e02f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "53339b758009a0896a87e9569cadcdb5a095ffe0c100cc7483d72e817e81b60b");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e\", 0], [h\"9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e\", 1]], 1: [[h\"010ba84307a0fcfc7a47dc6b747a2bcaf0a96e4373b86f10c3f019623fb9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d3\", 28467322]], 2: 183990, 3: 69885081, 4: [[0, [0, h\"b9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d3\"]], [2, [0, h\"b9bc72dd0341aed94b76e0f456c229ce77d0c96c91d97d6a4f5e23d3\"], h\"7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6\"]]}, {0: [[h\"65d6f4da4c7e6354664477078468751d5844603e0298d87b210a4a1b3084ff98\", h\"f2b392094a5bcc0663ddc26ce74fe2aa942fb9448ed46cf3ce3dfc08b23bf2414a136fd52956a564d8d19f8d613063969bc7169d7cfe41dd1219babab83eae05\"], [h\"92ca29f2ec2f81df5c2256f6715b35f9db0f2c9cde616d440ffbad420bfc75b0\", h\"85d5908503e17d69ea27809081e530a2f9c64e405469bb251648695b0626affe06ceb1d36b439675d38e46362f12e403adcecc4f94416a830de923672cb75e02\"]]}, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 3ul);
    }
}

TEST(CardanoStaking, RegisterStakingKey) {
    const auto privateKeyData = parse_hex(privateKeyTest1);
    const auto publicKey = PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, ownAddress1);
    const auto stakingKeyHash = subData(AddressV3(publicKey).bytes, 28, 28); // TODO obtain from API
    EXPECT_EQ(hex(stakingKeyHash), stakingKey1);
    const auto poolId = parse_hex(poolIdNufi);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(10000000ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_amount(5000000ul);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(69986091ul);

    // Register staking key, 2 ADA deposit
    input.mutable_register_staking_key()->set_staking_key(stakingKeyHash.data(), stakingKeyHash.size());
    input.mutable_register_staking_key()->set_deposit_amount(2000000ul);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a50081825820cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca01018182583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a007772fa021a00029f06031a042be72b048182008200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09ba100828258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840d08ed71da87d0928090edd9e226496ab109f2eee7926ac2ce51e7abe89a4f513c4afe2b85b71595e862e7f6fc992d14d2416a6e53a1961da7d26d3cf3f823400825820e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e06932584079ed55400cebc70c56ca87ba09009dfc298c64768f90a9139bf2e7f134250927c614ee846253fac33e652f1b50373d349fdfe13c207968c2a10991824fe2a10ef6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "6a206fe4df76e12499b4fd9722f33429f4d93f8a996f9f523fa6c02a8301386b");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca\", 1]], 1: [[h\"01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\", 7828218]], 2: 171782, 3: 69986091, 4: [[0, [0, h\"df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\"]]]}, {0: [[h\"6d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290\", h\"d08ed71da87d0928090edd9e226496ab109f2eee7926ac2ce51e7abe89a4f513c4afe2b85b71595e862e7f6fc992d14d2416a6e53a1961da7d26d3cf3f823400\"], [h\"e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e06932\", h\"79ed55400cebc70c56ca87ba09009dfc298c64768f90a9139bf2e7f134250927c614ee846253fac33e652f1b50373d349fdfe13c207968c2a10991824fe2a10e\"]]}, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 3ul);
    }
}

TEST(CardanoStaking, DeregisterStakingKey) {
    const auto privateKeyData = parse_hex(privateKeyTest1);
    const auto publicKey = PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, ownAddress1);
    const auto stakingKeyHash = subData(AddressV3(publicKey).bytes, 28, 28); // TODO obtain from API
    EXPECT_EQ(hex(stakingKeyHash), stakingKey1);
    const auto poolId = parse_hex(poolIdNufi);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(10000000ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_amount(5000000ul);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(69986091ul);

    // Deregister staking key, get back 2 ADA deposit
    input.mutable_deregister_staking_key()->set_staking_key(stakingKeyHash.data(), stakingKeyHash.size());
    input.mutable_deregister_staking_key()->set_undeposit_amount(2000000ul);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a50081825820cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca01018182583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a00b47bfa021a00029f06031a042be72b048182018200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09ba100828258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290584056619a7d6192b6f68c31a43e927c893161fd994d5c1bcc16f3710cf5e5e652e01f118d55f0110e9de34edc050d509748bea637db5c34f4fe342ae262ccb5520d825820e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e069325840d23680fdd8aa63e10efccc550eb726743b653008952f9d731d076d1df8106b0401823ebb195127b211389f1bc2c3f6ededbcec04bc8f0de93607a2409421e006f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "1caae2456e5471cc77e73410da475fb0a23874c18c1ea55f9267c59767caef0a");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca\", 1]], 1: [[h\"01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\", 11828218]], 2: 171782, 3: 69986091, 4: [[1, [0, h\"df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\"]]]}, {0: [[h\"6d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290\", h\"56619a7d6192b6f68c31a43e927c893161fd994d5c1bcc16f3710cf5e5e652e01f118d55f0110e9de34edc050d509748bea637db5c34f4fe342ae262ccb5520d\"], [h\"e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e06932\", h\"d23680fdd8aa63e10efccc550eb726743b653008952f9d731d076d1df8106b0401823ebb195127b211389f1bc2c3f6ededbcec04bc8f0de93607a2409421e006\"]]}, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 3ul);
    }
}

TEST(CardanoStaking, Redelegate) {
    const auto privateKeyData = parse_hex(privateKeyTest1);
    const auto publicKey = PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, ownAddress1);
    const auto stakingKeyHash = subData(AddressV3(publicKey).bytes, 28, 28); // TODO obtain from API
    EXPECT_EQ(hex(stakingKeyHash), stakingKey1);
    const auto poolId = parse_hex(poolIdNufi);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(10000000ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_amount(5000000ul);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(69986091ul);

    // Delegate, no deposit
    input.mutable_delegate()->set_staking_key(stakingKeyHash.data(), stakingKeyHash.size());
    input.mutable_delegate()->set_pool_id(poolId.data(), poolId.size());
    input.mutable_delegate()->set_deposit_amount(0ul);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a50081825820cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca01018182583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a0095f251021a0002a42f031a042be72b048183028200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b581c7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6a100828258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840fb48f3ddbfc2d4ca231a0581c5b456019aa4215ed5a2447ba89a4860569f9e7296afd3a0a81506882d8bda33683e623e6d8033786275369f7e247d866e017c06825820e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e069325840e26f696a6cd1c34101623568c9efe3796ff5855ada0e2e0cf557c7fc2148f6b2af176aff40a1f9c13fb29d9636c49f774d4a967c71f052f865cfaf0d02d5bb05f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "985f613fb8b86dad35f075599099776e50fc2a6aa74ee4b37c14fd9f2c0f0891");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"cba84549f07f2128410c0a22731f2c57f2a617746e8edc61b295cd8792638dca\", 1]], 1: [[h\"01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\", 9826897]], 2: 173103, 3: 69986091, 4: [[2, [0, h\"df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\"], h\"7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6\"]]}, {0: [[h\"6d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290\", h\"fb48f3ddbfc2d4ca231a0581c5b456019aa4215ed5a2447ba89a4860569f9e7296afd3a0a81506882d8bda33683e623e6d8033786275369f7e247d866e017c06\"], [h\"e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e06932\", h\"e26f696a6cd1c34101623568c9efe3796ff5855ada0e2e0cf557c7fc2148f6b2af176aff40a1f9c13fb29d9636c49f774d4a967c71f052f865cfaf0d02d5bb05\"]]}, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 3ul);
    }
}

TEST(CardanoStaking, RegisterAndDelegate_similar53339b) {
    const auto privateKeyData = parse_hex(privateKeyTest1);
    const auto publicKey = PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, ownAddress1);
    const auto stakingKeyHash = subData(AddressV3(publicKey).bytes, 28, 28); // TODO obtain from API
    EXPECT_EQ(hex(stakingKeyHash), stakingKey1);
    const auto poolId = parse_hex(poolIdNufi);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(4000000ul);
    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(1);
    utxo2->set_address(ownAddress);
    utxo2->set_amount(26651312ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_amount(4000000ul);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(69885081ul);

    // Register staking key, 2 ADA desposit
    input.mutable_register_staking_key()->set_staking_key(stakingKeyHash.data(), stakingKeyHash.size());
    input.mutable_register_staking_key()->set_deposit_amount(2000000ul);

    // Delegate
    input.mutable_delegate()->set_staking_key(stakingKeyHash.data(), stakingKeyHash.size());
    input.mutable_delegate()->set_pool_id(poolId.data(), poolId.size());
    input.mutable_delegate()->set_deposit_amount(0ul);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        const auto amount = 28475125ul;
        EXPECT_EQ(plan.availableAmount, 30651312ul);
        EXPECT_EQ(plan.amount, amount);
        EXPECT_EQ(plan.fee, 176187ul);
        EXPECT_EQ(plan.change, 30651312ul - 2000000ul - amount - 176187ul);

        // perform sign with default plan
        const auto output = signer.sign();

        EXPECT_EQ(output.error(), Common::Proto::OK);

        const auto encoded = data(output.encoded());
        EXPECT_EQ(hex(encoded), "83a500828258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e008258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e01018182583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a01b27ef5021a0002b03b031a042a5c99048282008200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b83028200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b581c7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6a100828258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840677c901704be027d9a1734e8aa06f0700009476fa252baaae0de280331746a320a61456d842d948ea5c0e204fc36f3bd04c88ca7ee3d657d5a38014243c37c07825820e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e0693258401fa21bdc62b85ca217bf08cbacdeba2fadaf33dc09ee3af9cc25b40f24822a1a42cfbc03585cc31a370ef75aaec4d25db6edcf329e40a4e725ec8718c94f220af6");
        const auto txid = data(output.tx_id());
        EXPECT_EQ(hex(txid), "96a781fd6481b6a7fd3926da110265e8c44b53947b81daa84da5b148825d02aa");
    }

    // set different plan, with exact fee
    const auto amount = 28467322ul;
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(28651312ul);
    input.mutable_plan()->set_fee(183990);
    input.mutable_plan()->set_change(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(1);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    // Similar to (but with different key): https://cardanoscan.io/transaction/53339b758009a0896a87e9569cadcdb5a095ffe0c100cc7483d72e817e81b60b
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a500828258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e008258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e01018182583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a01b2607a021a0002ceb6031a042a5c99048282008200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b83028200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b581c7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6a100828258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058405d8b21c993aec7a7bdf0c832e5688920b64b665e1e36a2e6040d6dd8ad195e7774df3c1377047737d8b676fa4115e38fbf6ef854904db6d9c8ee3e26e8561408825820e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e06932584088a3f6387693f9077d11a6e245e024b791074bcaa26c034e687d67f3324b6f90a437d33d0343e11c7dee1a28270c223e02080e452fe97cdc93d26c720ab6b805f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "23e1d1bc27f6de57e323d232d44c909fb41ee2ebfff28b82ca8cae6947866ea7");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e\", 0], [h\"9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e\", 1]], 1: [[h\"01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\", 28467322]], 2: 183990, 3: 69885081, 4: [[0, [0, h\"df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\"]], [2, [0, h\"df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\"], h\"7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6\"]]}, {0: [[h\"6d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290\", h\"5d8b21c993aec7a7bdf0c832e5688920b64b665e1e36a2e6040d6dd8ad195e7774df3c1377047737d8b676fa4115e38fbf6ef854904db6d9c8ee3e26e8561408\"], [h\"e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e06932\", h\"88a3f6387693f9077d11a6e245e024b791074bcaa26c034e687d67f3324b6f90a437d33d0343e11c7dee1a28270c223e02080e452fe97cdc93d26c720ab6b805\"]]}, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 3ul);
    }
}

} // namespace TW::Cardano::tests
