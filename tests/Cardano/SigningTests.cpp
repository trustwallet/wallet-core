// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Signer.h"
#include "Cardano/AddressV3.h"
#include "proto/Cardano.pb.h"

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


TEST(CardanoSigning, SelectInputsSimple) {
    const auto inputs = std::vector<TxInput>({
        TxInput{{parse_hex("0001"), 0}, "ad01", 700},
        TxInput{{parse_hex("0002"), 1}, "ad02", 900},
        TxInput{{parse_hex("0003"), 2}, "ad03", 300},
        TxInput{{parse_hex("0004"), 3}, "ad04", 600},
    });

    {   // 2
        const auto s1 = Signer::selectInputsSimple(inputs, 1500);
        ASSERT_EQ(s1.size(), 2);
        EXPECT_EQ(s1[0].amount, 900);
        EXPECT_EQ(s1[1].amount, 700);
    }
    {   // all
        const auto s1 = Signer::selectInputsSimple(inputs, 10000);
        ASSERT_EQ(s1.size(), 4);
        EXPECT_EQ(s1[0].amount, 900);
        EXPECT_EQ(s1[1].amount, 700);
        EXPECT_EQ(s1[2].amount, 600);
        EXPECT_EQ(s1[3].amount, 300);
    }
    {   // 3
        const auto s1 = Signer::selectInputsSimple(inputs, 2000);
        ASSERT_EQ(s1.size(), 3);
    }
    {   // 1
        const auto s1 = Signer::selectInputsSimple(inputs, 500);
        ASSERT_EQ(s1.size(), 1);
    }
    {   // at least 0 is returned
        const auto s1 = Signer::selectInputsSimple(inputs, 0);
        ASSERT_EQ(s1.size(), 1);
    }
}

Proto::SigningInput createSampleInput(uint64_t amount, int utxoCount = 10, const std::string& toAddress = "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5") {
    Proto::SigningInput input;
    if (utxoCount >= 1) {
        auto* utxo1 = input.add_utxos();
        const auto txHash1 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
        utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
        utxo1->mutable_out_point()->set_output_index(1);
        utxo1->set_address("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
        utxo1->set_amount(1500000);
    }
    if (utxoCount >= 2) {
        auto* utxo2 = input.add_utxos();
        const auto txHash2 = parse_hex("554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0");
        utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
        utxo2->mutable_out_point()->set_output_index(0);
        utxo2->set_address("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
        utxo2->set_amount(6500000);
    }

    const auto privateKeyData = parse_hex("089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e");
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address(toAddress);
    input.mutable_transfer_message()->set_change_address("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
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
        EXPECT_EQ(plan.fee, 170147);
        EXPECT_EQ(plan.change, 829853);
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
        EXPECT_EQ(plan.fee, 168390);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    {   // small target amount
        input.mutable_transfer_message()->set_amount(2000000);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 1);
        EXPECT_EQ(plan.availableAmount, 6500000);
        EXPECT_EQ(plan.amount, 2000000);
        EXPECT_EQ(plan.fee, 168565);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
}

TEST(CardanoSigning, SignTransfer1) {
    const auto input = createSampleInput(7000000);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca99d021a000298a3031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058403c1cbde706d10550f5f966a5071e9e01e18f7b555626ed7616a214bd951d29d0ff0458edf5c555eefe4d4280302160149fc6e7e56a4298a368b7ab357643190df6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "efcf8ec01fb8cd32bc5289c609c470b473cc79bc60b0667e1d68dc3962df1082");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0\", 0], [h\"f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767\", 1]], 1: [[h\"01558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5\", 7000000], [h\"01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\", 829853]], 2: 170147, 3: 53333333}, {0: [[h\"6d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290\", h\"3c1cbde706d10550f5f966a5071e9e01e18f7b555626ed7616a214bd951d29d0ff0458edf5c555eefe4d4280302160149fc6e7e56a4298a368b7ab357643190d\"]]}, null]");
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
        EXPECT_EQ(plan.fee, 168565);
        EXPECT_EQ(plan.change, 1831435);
        ASSERT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.utxos[0].amount, 6500000);
        EXPECT_EQ(plan.utxos[1].amount, 1500000);

        // perform sign with default plan
        const auto output = signer.sign();

        EXPECT_EQ(output.error(), Common::Proto::OK);

        const auto encoded = data(output.encoded());
        EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a005b8d8082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a001bf20b021a00029275031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290584064adab30f5a1db25db990819774c4ec6388e0bfaa6708ff27db1ac8a94fe8e4b769b2e2ef8b87d95896f04a0b97907f7dc7cc83cde85aea452084c1263625306f6");
        const auto txid = data(output.tx_id());
        EXPECT_EQ(hex(txid), "1cf53d225708b2cca137b25ae8c8a56fc2d599ed8418993322a3569468fd7c09");
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
        EXPECT_EQ(plan.amount, 8000000 - 170147);
        EXPECT_EQ(plan.fee, 170147);
        EXPECT_EQ(plan.change, 0);
        ASSERT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.utxos[0].amount, 1500000);
        EXPECT_EQ(plan.utxos[1].amount, 6500000);
    }

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000018182583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a0077795d021a000298a3031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058406594173f71e83ff34326376c4c03ca5abbecadd0de925bd13deaec98388f07006426b93ac34b1e1f2ad88ddd3bb658a7f2a2ddf72b04f2ccc2a9167917696304f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "6abeb64739837aaa2ae4ba261fa8573551e4840a09788b08d9274554a83011bd");
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
}

TEST(CardanoSigning, SignTransfer_0db1ea) {
    const auto amount = 1100000;
    const auto ownAddress = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23";

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("81b935447bb994567f041d181b628a0afbcd747d0199c9ff4cd895686bbee8c6");
    utxo1->mutable_out_point()->set_tx_hash(std::string(txHash1.begin(), txHash1.end()));
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(1000000);
    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("3a9068a273cc2af59b45593b78973841d972d01802abe992c55dbeecdffc561b");
    utxo2->mutable_out_point()->set_tx_hash(std::string(txHash2.begin(), txHash2.end()));
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_address(ownAddress);
    utxo2->set_amount(1800000);

    const auto privateKeyData1 = parse_hex("089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e");
    input.add_private_key(privateKeyData1.data(), privateKeyData1.size());
    input.mutable_transfer_message()->set_to_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_amount(amount);
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(54675589);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 1800000);
        EXPECT_EQ(plan.amount, amount);
        EXPECT_EQ(plan.fee, 168565);
        EXPECT_EQ(plan.change, 531435);
        EXPECT_EQ(plan.utxos.size(), 1);
    }

    // set plan (2 inputs)
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(2800000);
    input.mutable_plan()->set_fee(170147);
    input.mutable_plan()->set_change(1529853);
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
    input.mutable_transfer_message()->set_change_address("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
    input.mutable_transfer_message()->set_amount(7000000);
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(53333333);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_address);
    EXPECT_EQ(hex(output.encoded()), "");
}

TEST(CardanoSigning, SignTransferToLegacy) {
    const auto input = createSampleInput(7000000, 10, "Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_address);
    EXPECT_EQ(hex(output.encoded()), "");
}

TEST(CardanoSigning, SignTransferWithTokens) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_address("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
    utxo1->set_amount(8000000);
    auto* token = utxo1->add_token_amount();
    token->set_policy_id("testTokenPolicy1");
    token->set_asset_name("testTokenName1");
    const auto tokenAmount = store(uint256_t(20000000000));
    token->set_amount(tokenAmount.data(), tokenAmount.size());

    const auto privateKeyData = parse_hex("089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e");
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
    input.mutable_transfer_message()->set_change_address("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
    input.mutable_transfer_message()->set_amount(7000000);
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(53333333);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_utxo_amount);
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
    EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca99d021a000298a3031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058403c1cbde706d10550f5f966a5071e9e01e18f7b555626ed7616a214bd951d29d0ff0458edf5c555eefe4d4280302160149fc6e7e56a4298a368b7ab357643190df6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "efcf8ec01fb8cd32bc5289c609c470b473cc79bc60b0667e1d68dc3962df1082");
}

TEST(CardanoSigning, AnyPlan1) {
    const auto input = createSampleInput(7000000);

    Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeCardano);

    EXPECT_EQ(plan.error(), Common::Proto::OK);
    EXPECT_EQ(plan.amount(), 7000000);
    EXPECT_EQ(plan.available_amount(), 8000000);
    EXPECT_EQ(plan.fee(), 170147);
    EXPECT_EQ(plan.change(), 829853);
    ASSERT_EQ(plan.utxos_size(), 2);
    EXPECT_EQ(plan.utxos(0).amount(), 6500000);
    EXPECT_EQ(plan.utxos(1).amount(), 1500000);

    EXPECT_EQ(hex(plan.SerializeAsString()), "08c09fab031080a4e80318a3b10a209dd3322a92010a220a20554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af01267616464723171383034336d356865656179646e76746d6d6b7975686536717635686176766873663064323671336a7967737370786c796670796b3679716b77307968747976747230666c656b6a3834753634617a38326375666d716e36357a6473796c7a6b323318a0dd8c032a93010a240a20f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76710011267616464723171383034336d356865656179646e76746d6d6b7975686536717635686176766873663064323671336a7967737370786c796670796b3679716b77307968747976747230666c656b6a3834753634617a38326375666d716e36357a6473796c7a6b323318e0c65b");
}
