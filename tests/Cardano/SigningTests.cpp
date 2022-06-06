// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Signer.h"
#include "Cardano/AddressV3.h"
#include "proto/Cardano.pb.h"
#include <TrustWalletCore/TWCardano.h>

#include "PrivateKey.h"
#include "HexCoding.h"
#include "Cbor.h"
#include "uint256.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>
#include <vector>


using namespace TW::Cardano;
using namespace TW;
using namespace std;


const auto privateKeyTest1 = "089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e";
const auto ownAddress1 = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23";
const auto sundaeTokenPolicy = "9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77";

TEST(CardanoSigning, SelectInputs) {
    const auto inputs = std::vector<TxInput>({
        TxInput{{parse_hex("0001"), 0}, "ad01", 700},
        TxInput{{parse_hex("0002"), 1}, "ad02", 900},
        TxInput{{parse_hex("0003"), 2}, "ad03", 300},
        TxInput{{parse_hex("0004"), 3}, "ad04", 600},
    });

    {   // 2
        const auto s1 = Signer::selectInputsWithTokens(inputs, 1500, {});
        ASSERT_EQ(s1.size(), 2);
        EXPECT_EQ(s1[0].amount, 900);
        EXPECT_EQ(s1[1].amount, 700);
    }
    {   // all
        const auto s1 = Signer::selectInputsWithTokens(inputs, 10000, {});
        ASSERT_EQ(s1.size(), 4);
        EXPECT_EQ(s1[0].amount, 900);
        EXPECT_EQ(s1[1].amount, 700);
        EXPECT_EQ(s1[2].amount, 600);
        EXPECT_EQ(s1[3].amount, 300);
    }
    {   // 3
        const auto s1 = Signer::selectInputsWithTokens(inputs, 2000, {});
        ASSERT_EQ(s1.size(), 3);
    }
    {   // 1
        const auto s1 = Signer::selectInputsWithTokens(inputs, 500, {});
        ASSERT_EQ(s1.size(), 1);
    }
    {   // at least 0 is returned
        const auto s1 = Signer::selectInputsWithTokens(inputs, 0, {});
        ASSERT_EQ(s1.size(), 1);
    }
}

Proto::SigningInput createSampleInput(uint64_t amount, int utxoCount = 10, 
    const std::string& alternateToAddress = "", bool omitPrivateKey = false
) {
    const std::string toAddress = (alternateToAddress.length() > 0) ? alternateToAddress :
        "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5";

    Proto::SigningInput input;
    if (utxoCount >= 1) {
        auto* utxo1 = input.add_utxos();
        const auto txHash1 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
        utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
        utxo1->mutable_out_point()->set_output_index(1);
        utxo1->set_address(ownAddress1);
        utxo1->set_amount(1500000);
    }
    if (utxoCount >= 2) {
        auto* utxo2 = input.add_utxos();
        const auto txHash2 = parse_hex("554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0");
        utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
        utxo2->mutable_out_point()->set_output_index(0);
        utxo2->set_address(ownAddress1);
        utxo2->set_amount(6500000);
    }

    if (!omitPrivateKey) {
        const auto privateKeyData = parse_hex(privateKeyTest1);
        input.add_private_key(privateKeyData.data(), privateKeyData.size());
    }
    input.mutable_transfer_message()->set_to_address(toAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(amount);
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(53333333);
    return input;
}

TEST(CardanoSigning, Plan) {
    auto input = createSampleInput(7000000);

    {
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.availableAmount, 8000000);
        EXPECT_EQ(plan.amount, 7000000);
        EXPECT_EQ(plan.fee, 170196);
        EXPECT_EQ(plan.change, 829804);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
        EXPECT_EQ(plan.error, Common::Proto::OK);
    }
    {   // very small target amount
        input.mutable_transfer_message()->set_amount(1);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 1);
        EXPECT_EQ(plan.availableAmount, 6500000);
        EXPECT_EQ(plan.amount, 1);
        EXPECT_EQ(plan.fee, 168435);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    {   // small target amount
        input.mutable_transfer_message()->set_amount(2000000);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 1);
        EXPECT_EQ(plan.availableAmount, 6500000);
        EXPECT_EQ(plan.amount, 2000000);
        EXPECT_EQ(plan.fee, 168611);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    {   // small target amount requested, but max amount
        input.mutable_transfer_message()->set_amount(2000000);
        input.mutable_transfer_message()->set_use_max_amount(true);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.availableAmount, 8000000);
        EXPECT_EQ(plan.amount, 7832667);
        EXPECT_EQ(plan.fee, 167333);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
}

TEST(CardanoSigning, PlanForceFee) {
    auto requestedAmount = 6500000;
    auto availableAmount = 8000000;
    auto input = createSampleInput(requestedAmount);

    {
        auto fee = 170147;
        input.mutable_transfer_message()->set_force_fee(fee);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, requestedAmount);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, availableAmount - requestedAmount - fee);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
        EXPECT_EQ(plan.error, Common::Proto::OK);
    }
    {   // tiny fee
        auto fee = 100;
        input.mutable_transfer_message()->set_force_fee(fee);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, requestedAmount);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, availableAmount - requestedAmount - fee);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    {   // large fee
        auto fee = 1200000;
        input.mutable_transfer_message()->set_force_fee(fee);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, requestedAmount);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, availableAmount - requestedAmount - fee);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    {   // very large fee, larger than possible, truncated
        auto fee = 3000000;
        input.mutable_transfer_message()->set_force_fee(fee);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, requestedAmount);
        EXPECT_EQ(plan.fee, 1500000);
        EXPECT_EQ(plan.change, 0);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    {   // force fee and max amount: fee is used, amount is max, change 0
        auto fee = 160000;
        input.mutable_transfer_message()->set_force_fee(fee);
        input.mutable_transfer_message()->set_use_max_amount(true);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, 7840000);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, 0);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
}

TEST(CardanoSigning, PlanMissingPrivateKey) {
    auto input = createSampleInput(7000000, 10, "", true);

    auto signer = Signer(input);
    const auto plan = signer.doPlan();

    EXPECT_EQ(plan.utxos.size(), 2);
    EXPECT_EQ(plan.availableAmount, 8000000);
    EXPECT_EQ(plan.amount, 7000000);
    EXPECT_EQ(plan.fee, 170196);
    EXPECT_EQ(plan.change, 829804);
    EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    EXPECT_EQ(plan.error, Common::Proto::OK);
}

TEST(CardanoSigning, SignTransfer1) {
    const auto input = createSampleInput(7000000);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca96c021a000298d4031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058407cf591599852b5f5e007fdc241062405c47e519266c0d884b0767c1d4f5eacce00db035998e53ed10ca4ba5ce4aac8693798089717ce6cf4415f345cc764200ef6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "9b5b15e133cd73ccaa85307d2986aebc846505118a2eb4e6111e6b4b67d1f389");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0\", 0], [h\"f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767\", 1]], 1: [[h\"01558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5\", 7000000], [h\"01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\", 829804]], 2: 170196, 3: 53333333}, {0: [[h\"6d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290\", h\"7cf591599852b5f5e007fdc241062405c47e519266c0d884b0767c1d4f5eacce00db035998e53ed10ca4ba5ce4aac8693798089717ce6cf4415f345cc764200e\"]]}, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 3);
    }
}

TEST(CardanoSigning, PlanAndSignTransfer1) {
    uint amount = 6000000;
    auto input = createSampleInput(amount);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 8000000);
        EXPECT_EQ(plan.amount, amount);
        EXPECT_EQ(plan.fee, 170196);
        EXPECT_EQ(plan.change, 8000000 - amount - 170196);
        ASSERT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.utxos[0].amount, 6500000);
        EXPECT_EQ(plan.utxos[1].amount, 1500000);

        // perform sign with default plan
        const auto output = signer.sign();

        EXPECT_EQ(output.error(), Common::Proto::OK);

        const auto encoded = data(output.encoded());
        EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a005b8d8082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a001bebac021a000298d4031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058404abc749ffaffcf2f87970e4f1983c5e44b352ee1515b60017fc65e581d42b3a6ed146d5eb35d04a770460b0541a25afd5aedfd027fdaded82686f43454196a0cf6");
        const auto txid = data(output.tx_id());
        EXPECT_EQ(hex(txid), "3852f809245d7000ad0c5ccb1357e5d333b0dd25158924581e4c7049ec68c564");
    }

    // set different plan, with one input only
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(6500000);
    input.mutable_plan()->set_fee(165489);
    input.mutable_plan()->set_change(17191988);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40081825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a005b8d8082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a01065434021a00028671031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058408311a058035d75545a47b844fea401aa9c23e99fe7bc8136b554396eef135d4cd93062c5df38e613185c21bb1c98b881d1e0fd1024d3539b163c8e14d1a6e40df6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "e319c0bfc99cdb79d64f00b7e8fb8bfbf29fa70554c84f101e92b7dfed172448");
}


TEST(CardanoSigning, PlanAndSignMaxAmount) {
    auto input = createSampleInput(7000000);
    input.mutable_transfer_message()->set_use_max_amount(true);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 8000000);
        EXPECT_EQ(plan.amount, 8000000 - 167333);
        EXPECT_EQ(plan.fee, 167333);
        EXPECT_EQ(plan.change, 0);
        ASSERT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.utxos[0].amount, 1500000);
        EXPECT_EQ(plan.utxos[1].amount, 6500000);
    }

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000018182583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a0077845b021a00028da5031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058403e64473e08adc863953c0e9f820b658dda0b8a423d6172fdccff73fcd5559956c9df8ed93ff67405331d368a0c11fd18c69781046384946582e1555e9e8ec70bf6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "ca0f1e12f20c95011da7d686d206a1eb98df94accd74c4df4ef403c5ce836057");
}

TEST(CardanoSigning, SignNegative) {
    {   // plan with error
        auto input = createSampleInput(7000000);
        const auto error = Common::Proto::Error_invalid_memo;
        input.mutable_plan()->set_error(error);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), error);
    }
    {   // zero requested amount
        auto input = createSampleInput(0);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), Common::Proto::Error_zero_amount_requested);
    }
    {   // no utxo
        auto input = createSampleInput(7000000, 0);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), Common::Proto::Error_missing_input_utxos);
    }
    {   // low balance
        auto input = createSampleInput(7000000000);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), Common::Proto::Error_low_balance);
    }
    {   // missing private key
        auto input = createSampleInput(7000000, 10, "", true);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), Common::Proto::Error_missing_private_key);
    }
}

TEST(CardanoSigning, SignTransfer_0db1ea) {
    const auto amount = 1100000;

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("81b935447bb994567f041d181b628a0afbcd747d0199c9ff4cd895686bbee8c6");
    utxo1->mutable_out_point()->set_tx_hash(std::string(txHash1.begin(), txHash1.end()));
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress1);
    utxo1->set_amount(1000000);
    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("3a9068a273cc2af59b45593b78973841d972d01802abe992c55dbeecdffc561b");
    utxo2->mutable_out_point()->set_tx_hash(std::string(txHash2.begin(), txHash2.end()));
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_address(ownAddress1);
    utxo2->set_amount(1800000);

    const auto privateKeyData1 = parse_hex(privateKeyTest1);
    input.add_private_key(privateKeyData1.data(), privateKeyData1.size());
    input.mutable_transfer_message()->set_to_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(amount);
    auto fee = 170147;
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.mutable_transfer_message()->set_force_fee(fee); // use force fee feature here
    input.set_ttl(54675589);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 2800000);
        EXPECT_EQ(plan.amount, amount);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, 2800000 - amount - fee);
        EXPECT_EQ(plan.utxos.size(), 2);
    }

    // set plan with specific fee, to match the real transaction
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(2800000);
    input.mutable_plan()->set_fee(fee);
    input.mutable_plan()->set_change(2800000 - amount - fee);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(1);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    // https://cardanoscan.io/transaction/0db1ea8c5c5828bbd027fcef3da02a63b86899db670ad7bb0630cefbe35944fa
    // curl -d '{"txHash":"0db1ea..44fa","txBody":"83a400..06f6"}' -H "Content-Type: application/json" https://<cardano-node>/api/txs/submit
    EXPECT_EQ(output.error(), Common::Proto::OK);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a4008282582081b935447bb994567f041d181b628a0afbcd747d0199c9ff4cd895686bbee8c6008258203a9068a273cc2af59b45593b78973841d972d01802abe992c55dbeecdffc561b000182825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a34681a0010c8e082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a001757fd021a000298a3031a03424885a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058406300b52aaff1e26067a3e0a48ae26f4f068765f46f934fabeab872c1d25535fc94893ec72feacd787f0174fbabd8933727d9a2b319b406e7a855843b0c051806f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "0db1ea8c5c5828bbd027fcef3da02a63b86899db670ad7bb0630cefbe35944fa");
}

TEST(CardanoSigning, SignTransferFromLegacy) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_address("Ae2tdPwUPEZMRgecV9jV2e9RdbrmnWu7YgRie4de16xLdkWhy6q7ypmRhgn");
    utxo1->set_amount(1500000);
    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_address("Ae2tdPwUPEZMRgecV9jV2e9RdbrmnWu7YgRie4de16xLdkWhy6q7ypmRhgn");
    utxo2->set_amount(6500000);

    const auto privateKeyData = parse_hex("c031e942f6bf2b2864700e7da20964ee6bb6d716345ce2e24d8c00e6500b574411111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
    {
        const auto privKey = PrivateKey(privateKeyData);
        const auto pubKey = privKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        const auto addr = AddressV2(pubKey);
        EXPECT_EQ(addr.string(), "Ae2tdPwUPEZMRgecV9jV2e9RdbrmnWu7YgRie4de16xLdkWhy6q7ypmRhgn");
    }
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(7000000);
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(53333333);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_address);
    EXPECT_EQ(hex(output.encoded()), "");
}

TEST(CardanoSigning, SignTransferToLegacy) {
    const auto toAddressLegacy = "DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ";
    EXPECT_FALSE(AddressV3::isValid(toAddressLegacy)); // not V3
    EXPECT_TRUE(AddressV3::isValidLegacy(toAddressLegacy));

    const auto input = createSampleInput(7000000, 10, toAddressLegacy);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);
    EXPECT_EQ(hex(output.encoded()), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282584c82d818584283581c6aebd89cf88271c3ee76339930d8956b03f018b2f4871522f88eb8f9a101581e581c692a37dae3bc63dfc3e1463f12011f26655ab1d1e0f4ed4b8fc63708001ad8a9555b1a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca627021a00029c19031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840db9becdc733f4c08c0e7abc29b5cc6469f9339d32f565df8bf77455439ae1f949facc9b831754e74d3fbb42e99647eedd6c28de1461d18c315485f5d24b5b90af6");
    EXPECT_EQ(hex(data(output.tx_id())), "f9b713e9987ec1377ac223f50d63c7a5e155915302de43f40d7b2627accabf69");
}

TEST(CardanoSigning, SignTransferToInvalid) {
    const auto input = createSampleInput(7000000, 10, "__INVALID_ADDRESS__");

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_address);
    EXPECT_EQ(hex(output.encoded()), "");
}

TEST(CardanoSigning, SignTransferToken) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_address(ownAddress1);
    utxo1->set_amount(8051373);
    // some token, to be preserved
    auto* token3 = utxo1->add_token_amount();
    token3->set_policy_id(sundaeTokenPolicy);
    token3->set_asset_name("CUBY");
    const auto tokenAmount3 = store(uint256_t(3000000));
    token3->set_amount(tokenAmount3.data(), tokenAmount3.size());

    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(2);
    utxo2->set_address(ownAddress1);
    utxo2->set_amount(2000000);
    // some SUNDAE token, to be transferred
    auto* token1 = utxo2->add_token_amount();
    token1->set_policy_id(sundaeTokenPolicy);
    token1->set_asset_name("SUNDAE");
    const auto tokenAmount1 = store(uint256_t(80996569));
    token1->set_amount(tokenAmount1.data(), tokenAmount1.size());
    // some other token, to be preserved
    auto* token2 = utxo2->add_token_amount();
    token2->set_policy_id(sundaeTokenPolicy);
    token2->set_asset_name("CUBY");
    const auto tokenAmount2 = store(uint256_t(2000000));
    token2->set_amount(tokenAmount2.data(), tokenAmount2.size());

    const auto privateKeyData = parse_hex(privateKeyTest1);
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
    input.mutable_transfer_message()->set_change_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    input.mutable_transfer_message()->set_amount(1500000);
    auto* toToken = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    toToken->set_policy_id(sundaeTokenPolicy);
    toToken->set_asset_name("SUNDAE");
    const auto toTokenAmount = store(uint256_t(20000000));
    toToken->set_amount(toTokenAmount.data(), toTokenAmount.size());
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(53333333);

    {   // check min ADA amount, set it
        const auto bundleProtoData = data(input.transfer_message().token_amount().SerializeAsString());
        const auto minAdaAmount = TWCardanoMinAdaAmount(&bundleProtoData);
        EXPECT_EQ(minAdaAmount, 1444443);
        input.mutable_transfer_message()->set_amount(minAdaAmount);
    }

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 10051373);
        EXPECT_EQ(plan.amount, 1444443);
        EXPECT_EQ(plan.fee, 175966);
        EXPECT_EQ(plan.change, 8430964);
        EXPECT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.availableTokens.size(), 2);
        EXPECT_EQ(plan.availableTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_CUBY"), 5000000);
        EXPECT_EQ(plan.availableTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 80996569);
        EXPECT_EQ(plan.outputTokens.size(), 1);
        EXPECT_EQ(plan.outputTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_CUBY"), 0);
        EXPECT_EQ(plan.outputTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 20000000);
        EXPECT_EQ(plan.changeTokens.size(), 2);
        EXPECT_EQ(plan.changeTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_CUBY"), 5000000);
        EXPECT_EQ(plan.changeTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 60996569);
    }

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76702018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5821a00160a5ba1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a01312d00825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468821a0080a574a2581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a144435542591a004c4b40581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a03a2bbd9021a0002af5e031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290584000feb412442f8851faa59742eb2c37f3994b0d143a424367143490cf828246991e504fa8eac61c403bfa7634bd1f0adc44f3f54f6a474856701e2cbb15fb5b04f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "dacb3a0c5b3b7fa36b49f25a0a59b941ab8a21f0db5770e9e6982ff120122649");

    {
        // also test proto toProto / fromProto
        const Proto::TransactionPlan planProto = Signer::plan(input);
        const auto plan2 = TransactionPlan::fromProto(planProto);
        EXPECT_EQ(plan2.amount, 1444443);
        EXPECT_EQ(plan2.change, 8430964);
    }
}

TEST(CardanoSigning, SignTransferToken_1dd248) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("f2d2b11c8c07c5c646f5b5af20fddf2f0a174743c6a1b13cca27e28a6ca34710");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress1);
    utxo1->set_amount(1500000);
    // some token
    auto* token3 = utxo1->add_token_amount();
    token3->set_policy_id(sundaeTokenPolicy);
    token3->set_asset_name("SUNDAE");
    const auto tokenAmount3 = store(uint256_t(20000000));
    token3->set_amount(tokenAmount3.data(), tokenAmount3.size());

    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("6975fcf7bbca745c85f50777f956219868fd9cad14ba496fed1371252e8df60f");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_address(ownAddress1);
    utxo2->set_amount(10258890);

    const auto privateKeyData = parse_hex(privateKeyTest1);
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq"); // Test
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(1600000);
    auto* toToken = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    toToken->set_policy_id(sundaeTokenPolicy);
    toToken->set_asset_name("SUNDAE");
    const auto toTokenAmount = store(uint256_t(11000000));
    toToken->set_amount(toTokenAmount.data(), toTokenAmount.size());
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(61232158);

    {   // check min ADA amount
        const auto bundleProtoData = data(input.transfer_message().token_amount().SerializeAsString());
        EXPECT_EQ(TWCardanoMinAdaAmount(&bundleProtoData), 1444443);
        EXPECT_GT(input.transfer_message().amount(), TWCardanoMinAdaAmount(&bundleProtoData));
    }

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 11758890);
        EXPECT_EQ(plan.amount, 11758890 - 9984729 - 174161);
        EXPECT_EQ(plan.fee, 174161);
        EXPECT_EQ(plan.change, 9984729);
        EXPECT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.availableTokens.size(), 1);
        EXPECT_EQ(plan.availableTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 20000000);
        EXPECT_EQ(plan.outputTokens.size(), 1);
        EXPECT_EQ(plan.outputTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 11000000);
        EXPECT_EQ(plan.changeTokens.size(), 1);
        EXPECT_EQ(plan.changeTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 9000000);
    }

    // set plan with specific fee, to match the real transaction
    input.mutable_plan()->set_available_amount(11758890);
    input.mutable_plan()->set_amount(1600000);
    input.mutable_plan()->set_fee(174102);
    input.mutable_plan()->set_change(9984788);
    *(input.mutable_plan()->add_available_tokens()) = input.utxos(0).token_amount(0);
    *(input.mutable_plan()->add_output_tokens()) = input.utxos(0).token_amount(0);
    input.mutable_plan()->mutable_output_tokens(0)->set_amount(toTokenAmount.data(), toTokenAmount.size());
    *(input.mutable_plan()->add_change_tokens()) = input.utxos(0).token_amount(0);
    const auto changeTokenAmount = store(uint256_t(9000000));
    input.mutable_plan()->mutable_change_tokens(0)->set_amount(changeTokenAmount.data(), changeTokenAmount.size());
    *(input.mutable_plan()->add_utxos()) = input.utxos(1);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    // https://cardanoscan.io/transaction/1dd24872d93d3b5091b98e19b9f920cd0c4369e4c5ca178e898152c52f00c162
    // curl -d '{"txHash":"1dd248..c162","txBody":"83a400..08f6"}' -H "Content-Type: application/json" https://<cardano-node>/api/txs/submit
    EXPECT_EQ(output.error(), Common::Proto::OK);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a400828258206975fcf7bbca745c85f50777f956219868fd9cad14ba496fed1371252e8df60f00825820f2d2b11c8c07c5c646f5b5af20fddf2f0a174743c6a1b13cca27e28a6ca34710000182825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468821a00186a00a1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a00a7d8c082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b821a00985b14a1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a00895440021a0002a816031a03a6541ea100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840c8cdee32bfd584f55cf334b4ec6f734635144736d48f882e647a7a6283f230bc5a67d4dd66a9e523e0c29c812ed1e3589febbcf96547a1fc6d061a7ccfb81308f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "1dd24872d93d3b5091b98e19b9f920cd0c4369e4c5ca178e898152c52f00c162");
}

TEST(CardanoSigning, SignTransferTokenMaxAmount_620b71) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("46964521ad00d9b3f3d41f77c07e1b3093848048dbdf2d95cf900e15cdac0d7f");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress1);
    utxo1->set_amount(2170871);
    // some token
    auto* token1 = utxo1->add_token_amount();
    token1->set_policy_id(sundaeTokenPolicy);
    token1->set_asset_name("SUNDAE");
    const auto tokenAmount1 = store(uint256_t(20000000));
    token1->set_amount(tokenAmount1.data(), tokenAmount1.size());

    const auto privateKeyData = parse_hex(privateKeyTest1);
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(666); // doesn't matter, max is used
    auto* toToken = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    toToken->set_policy_id(sundaeTokenPolicy);
    toToken->set_asset_name("SUNDAE");
    const auto toTokenAmount = store(uint256_t(666));  // doesn't matter, max is used
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(61085916);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 2170871);
        EXPECT_EQ(plan.amount, 2170871 - 167730);
        EXPECT_EQ(plan.fee, 167730);
        EXPECT_EQ(plan.change, 0);
        EXPECT_EQ(plan.utxos.size(), 1);
        EXPECT_EQ(plan.availableTokens.size(), 1);
        EXPECT_EQ(plan.availableTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 20000000);
        EXPECT_EQ(plan.outputTokens.size(), 1);
        EXPECT_EQ(plan.outputTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 20000000);
        EXPECT_EQ(plan.changeTokens.size(), 0);
    }

    // set plan with specific fee, to match the real transaction
    input.mutable_plan()->set_available_amount(2170871);
    input.mutable_plan()->set_amount(1998526);
    input.mutable_plan()->set_fee(172345);
    input.mutable_plan()->set_change(0);
    *(input.mutable_plan()->add_available_tokens()) = input.utxos(0).token_amount(0);
    *(input.mutable_plan()->add_output_tokens()) = input.utxos(0).token_amount(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    // https://cardanoscan.io/transaction/620b719338efb419b0e1417bfbe01fc94a62d5669a4b8cbbf4e32ecc1ca3b872
    // curl -d '{"txHash":"620b71..b872","txBody":"83a400..08f6"}' -H "Content-Type: application/json" https://<cardano-node>/api/txs/submit
    EXPECT_EQ(output.error(), Common::Proto::OK);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a4008182582046964521ad00d9b3f3d41f77c07e1b3093848048dbdf2d95cf900e15cdac0d7f00018182583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5821a001e7ebea1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a01312d00021a0002a139031a03a418dca100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840e1d1565cd747b20b0f10a92f068f3d5faebdee92b4b4a4b96ce14736d975e17d1446f7f51e64997a0bb38e0151dc738468161d574d6cfcd8040e4455ff46bc08f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "620b719338efb419b0e1417bfbe01fc94a62d5669a4b8cbbf4e32ecc1ca3b872");
}

TEST(CardanoSigning, SignTransferTwoTokens) {
    auto input = createSampleInput(7000000);
    input.mutable_transfer_message()->set_amount(1500000);
    auto* token1 = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    token1->set_policy_id(sundaeTokenPolicy);
    token1->set_asset_name("SUNDAE");
    const auto tokenAmount1 = store(uint256_t(40000000));
    token1->set_amount(tokenAmount1.data(), tokenAmount1.size());
    auto* token2 = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    token2->set_policy_id(sundaeTokenPolicy);
    token2->set_asset_name("CUBY");
    const auto tokenAmount2 = store(uint256_t(2000000));
    token2->set_amount(tokenAmount2.data(), tokenAmount2.size());

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_requested_token_amount);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(output.encoded()), "");
}

TEST(CardanoSigning, SignMessageWithKey) {
    // test case from cardano-crypto.js

    const auto privateKey = PrivateKey(parse_hex(
        "d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"
        "d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"
        "69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "1111111111111111111111111111111111111111111111111111111111111111"
    ));

    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
    EXPECT_EQ(hex(publicKey.bytes), 
        "e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf698"
        "69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"
        "857eed804ff087b97f87848f6493e87257a8c5203cb9f422f6e7a7d8a4d299f3"
        "1111111111111111111111111111111111111111111111111111111111111111"
    );

    const auto sampleMessageStr = "Hello world";
    const auto sampleMessage = data(sampleMessageStr);

    const auto signature = privateKey.sign(sampleMessage, TWCurveED25519Extended);

    const auto sampleRightSignature = "1096ddcfb2ad21a4c0d861ef3fabe18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d829bf0bcf1b631e86f0e";
    EXPECT_EQ(hex(signature), sampleRightSignature);
}

TEST(CardanoSigning, AnySignTransfer1) {
    const auto input = createSampleInput(7000000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeCardano);

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca96c021a000298d4031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058407cf591599852b5f5e007fdc241062405c47e519266c0d884b0767c1d4f5eacce00db035998e53ed10ca4ba5ce4aac8693798089717ce6cf4415f345cc764200ef6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "9b5b15e133cd73ccaa85307d2986aebc846505118a2eb4e6111e6b4b67d1f389");
}

TEST(CardanoSigning, AnyPlan1) {
    const auto input = createSampleInput(7000000);

    Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeCardano);

    EXPECT_EQ(plan.error(), Common::Proto::OK);
    EXPECT_EQ(plan.amount(), 7000000);
    EXPECT_EQ(plan.available_amount(), 8000000);
    EXPECT_EQ(plan.fee(), 170196);
    EXPECT_EQ(plan.change(), 829804);
    ASSERT_EQ(plan.utxos_size(), 2);
    EXPECT_EQ(plan.utxos(0).amount(), 6500000);
    EXPECT_EQ(plan.utxos(1).amount(), 1500000);

    EXPECT_EQ(hex(plan.SerializeAsString()), "0880a4e80310c09fab0318d4b10a20ecd2324292010a220a20554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af01267616464723171383034336d356865656179646e76746d6d6b7975686536717635686176766873663064323671336a7967737370786c796670796b3679716b77307968747976747230666c656b6a3834753634617a38326375666d716e36357a6473796c7a6b323318a0dd8c034293010a240a20f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76710011267616464723171383034336d356865656179646e76746d6d6b7975686536717635686176766873663064323671336a7967737370786c796670796b3679716b77307968747976747230666c656b6a3834753634617a38326375666d716e36357a6473796c7a6b323318e0c65b");

    {
        // also test fromProto
        const auto plan2 = TransactionPlan::fromProto(plan);
        EXPECT_EQ(plan2.amount, plan.amount());
        EXPECT_EQ(plan2.change, plan.change());
    }
}
