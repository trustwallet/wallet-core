// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Nervos/Address.h"
#include "Nervos/Serialization.h"
#include "Nervos/TransactionPlan.h"
#include "PublicKey.h"
#include "proto/Nervos.pb.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

namespace TW::Nervos::tests {

Proto::SigningInput getAnySignerInput1() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_native_transfer();

    operation.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02"
                             "wectaumxn0664yw2jd53lqk4mxg3");
    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama");
    operation.set_amount(10000000000);
    input.set_byte_fee(1);

    auto& cell1 = *input.add_cell();
    cell1.set_capacity(100000000000);
    *cell1.mutable_out_point() =
        OutPoint(parse_hex("71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3"), 1)
            .proto();
    *cell1.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();

    auto& cell2 = *input.add_cell();
    cell2.set_capacity(20000000000);
    *cell2.mutable_out_point() =
        OutPoint(parse_hex("71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3"), 0)
            .proto();
    *cell2.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();

    auto privateKey = parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb");
    input.add_private_key(std::string(privateKey.begin(), privateKey.end()));

    return input;
}

void checkAnySignerOutput1(Proto::SigningOutput& output) {
    ASSERT_EQ(output.error(), Common::Proto::OK);
    // https://explorer.nervos.org/transaction/0xf2c32afde7e72011985583873bc16c0a3c01fc01fc161eb4b914fcf84c53cdf8
    ASSERT_EQ(output.transaction_id(),
              "0xf2c32afde7e72011985583873bc16c0a3c01fc01fc161eb4b914fcf84c53cdf8");
    ASSERT_EQ(
        output.transaction_json(),
        "{\"cell_deps\":[{\"dep_type\":\"dep_group\",\"out_point\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c\"}}],\"header_deps\":"
        "[],\"inputs\":[{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0x71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3\"},\"since\":\"0x0\"}"
        "],\"outputs\":[{\"capacity\":\"0x2540be400\",\"lock\":{\"args\":"
        "\"0xab201f55b02f53b385f79b34dfad548e549b48fc\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":null},{\"capacity\":\"0x2540be230\",\"lock\":{\"args\":"
        "\"0xb0d65be39059d6489231b48f85ad706a560bbd8d\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":null}],\"outputs_data\":[\"0x\",\"0x\"],\"version\":\"0x0\","
        "\"witnesses\":["
        "\"0x55000000100000005500000055000000410000002a9ef2ad7829e5ea0c7a32735d29a0cb2ec20434f6fd5b"
        "f6e29cda56b28e08140156191cbbf80313d3c9cae4b74607acce7b28eb21d52ef058ed8491cdde70b700\"]}");
}

void checkPlan1(Proto::TransactionPlan& txPlanProto) {
    ASSERT_EQ(txPlanProto.error(), Common::Proto::OK);

    ASSERT_EQ(txPlanProto.cell_deps_size(), 1);

    const auto cellDep1 = txPlanProto.cell_deps(0);
    const auto cellDep1TxHash =
        parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c");
    ASSERT_EQ(cellDep1.out_point().tx_hash(),
              std::string(cellDep1TxHash.begin(), cellDep1TxHash.end()));
    ASSERT_EQ(cellDep1.out_point().index(), 0ul);
    ASSERT_EQ(cellDep1.dep_type(), "dep_group");

    ASSERT_EQ(txPlanProto.header_deps_size(), 0);

    ASSERT_EQ(txPlanProto.selected_cells_size(), 1);

    auto cell1 = Cell(txPlanProto.selected_cells(0));
    ASSERT_EQ(cell1.outPoint.txHash,
              parse_hex("71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3"));
    ASSERT_EQ(cell1.outPoint.index, 0ul);
    ASSERT_EQ(cell1.capacity, 20000000000ul);
    ASSERT_EQ(cell1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cell1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cell1.lock.args, parse_hex("c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da"));
    ASSERT_EQ(cell1.type.codeHash.size(), 0ul);
    ASSERT_EQ(cell1.type.args.size(), 0ul);
    ASSERT_EQ(cell1.data.size(), 0ul);

    ASSERT_EQ(txPlanProto.outputs_size(), 2);
    ASSERT_EQ(txPlanProto.outputs_data_size(), 2);

    auto cellOutput1 = CellOutput(txPlanProto.outputs(0));
    ASSERT_EQ(cellOutput1.capacity, 10000000000ul);
    ASSERT_EQ(cellOutput1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(cellOutput1.type.codeHash.size(), 0ul);
    ASSERT_EQ(cellOutput1.type.args.size(), 0ul);
    ASSERT_EQ(txPlanProto.outputs_data(0).length(), 0ul);

    auto cellOutput2 = CellOutput(txPlanProto.outputs(1));
    ASSERT_EQ(cellOutput2.capacity, 9999999536ul);
    ASSERT_EQ(cellOutput2.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput2.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput2.lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(cellOutput2.type.codeHash.size(), 0ul);
    ASSERT_EQ(cellOutput2.type.args.size(), 0ul);
    ASSERT_EQ(txPlanProto.outputs_data(1).length(), 0ul);
}

TEST(TWAnySignerNervos, Sign_Native_Simple) {
    auto input = getAnySignerInput1();
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    checkAnySignerOutput1(output);
}

TEST(TWAnySignerNervos, PlanAndSign_Native_Simple) {
    auto input = getAnySignerInput1();
    Proto::TransactionPlan txPlanProto;
    ANY_PLAN(input, txPlanProto, TWCoinTypeNervos);
    checkPlan1(txPlanProto);
    *input.mutable_plan() = txPlanProto;
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    checkAnySignerOutput1(output);
}

TEST(TWAnySignerNervos, Sign_NegativeMissingKey) {
    auto input = getAnySignerInput1();
    input.clear_private_key();
    auto privateKey = parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61fec");
    input.add_private_key(std::string(privateKey.begin(), privateKey.end()));
    Proto::SigningOutput output;

    ANY_SIGN(input, TWCoinTypeNervos);

    ASSERT_EQ(output.error(), Common::Proto::Error_missing_private_key);
}

TEST(TWAnySignerNervos, Sign_NegativeNotEnoughUtxos) {
    auto input = getAnySignerInput1();
    auto& operation = *input.mutable_native_transfer();
    operation.set_amount(1000000000000);
    Proto::SigningOutput output;

    ANY_SIGN(input, TWCoinTypeNervos);

    ASSERT_EQ(output.error(), Common::Proto::Error_not_enough_utxos);
}

Proto::SigningInput getAnySignerInput2() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_native_transfer();

    operation.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02"
                             "wectaumxn0664yw2jd53lqk4mxg3");
    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama");
    operation.set_use_max_amount(true);
    input.set_byte_fee(1);

    auto& cell1 = *input.add_cell();
    cell1.set_capacity(11410040620);
    *cell1.mutable_out_point() =
        OutPoint(parse_hex("c75567c80dc9b97aaf4e5c23f4c7f37b077f2b33a50dd7abd952abfbd5beb247"), 0)
            .proto();
    *cell1.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();

    auto privateKey = parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb");
    input.add_private_key(std::string(privateKey.begin(), privateKey.end()));

    return input;
}

TEST(TWAnySignerNervos, Sign_Native_SendMaximum) {
    auto input = getAnySignerInput2();
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    ASSERT_EQ(output.error(), Common::Proto::OK);
    // https://explorer.nervos.org/transaction/0x298f5e04b6900796614b89062eb96cec63c3b2c460d01058736a793b567bc5c8
    ASSERT_EQ(output.transaction_id(),
              "0x298f5e04b6900796614b89062eb96cec63c3b2c460d01058736a793b567bc5c8");
    ASSERT_EQ(
        output.transaction_json(),
        "{\"cell_deps\":[{\"dep_type\":\"dep_group\",\"out_point\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c\"}}],\"header_deps\":"
        "[],\"inputs\":[{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0xc75567c80dc9b97aaf4e5c23f4c7f37b077f2b33a50dd7abd952abfbd5beb247\"},\"since\":\"0x0\"}"
        "],\"outputs\":[{\"capacity\":\"0x2a81765c9\",\"lock\":{\"args\":"
        "\"0xab201f55b02f53b385f79b34dfad548e549b48fc\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":null}],\"outputs_data\":[\"0x\"],\"version\":\"0x0\",\"witnesses\":["
        "\"0x5500000010000000550000005500000041000000daf6e65e5a1fe447a4feb7199886b6635c44738e04ea59"
        "457608fb1c447e068026529d57b02014ddc144622f886153df426853f22083f8891461eeb50b5ce97d01\"]}");
}

Proto::SigningInput getAnySignerInput3() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_sudt_transfer();

    operation.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02"
                             "wectaumxn0664yw2jd53lqk4mxg3");
    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama");
    uint256_t amount = 1000000000000000;
    operation.set_amount(toString(amount));
    input.set_byte_fee(1);
    auto sudtAddress =
        parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8");
    *operation.mutable_sudt_address() = std::string(sudtAddress.begin(), sudtAddress.end());

    auto& cell1 = *input.add_cell();
    cell1.set_capacity(14399998906);
    *cell1.mutable_out_point() =
        OutPoint(parse_hex("5b12911e7413e011f251c1fb5fae4e76fd5fcae4f0d4c6412dcc5b0bfcece823"), 0)
            .proto();
    *cell1.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();

    auto& cell2 = *input.add_cell();
    cell2.set_capacity(14400000000);
    *cell2.mutable_out_point() =
        OutPoint(parse_hex("e118bd11a73d381daf288381ce182d92b6cf2f52d25886bbda9e1a61525c7c4a"), 0)
            .proto();
    *cell2.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();
    *cell2.mutable_type() =
        Script(parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5"),
               HashType::Type1,
               parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8"))
            .proto();
    auto cell2Data = parse_hex("00e0e4c9b9f84f000000000000000000");
    *cell2.mutable_data() = std::string(cell2Data.begin(), cell2Data.end());

    auto& cell3 = *input.add_cell();
    cell3.set_capacity(8210025567);
    *cell3.mutable_out_point() =
        OutPoint(parse_hex("09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a"), 1)
            .proto();
    *cell3.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqds6ed78yze6eyfyvd537z66ur22c9mmrgz82ama"))
                                .proto();

    auto privateKey1 =
        parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb");
    input.add_private_key(std::string(privateKey1.begin(), privateKey1.end()));
    auto privateKey2 =
        parse_hex("0c8859a9d9084a8c2b55963268b352e258756f9240f2a1f4645c610ed191dae9");
    input.add_private_key(std::string(privateKey2.begin(), privateKey2.end()));

    return input;
}

TEST(TWAnySignerNervos, Sign_SUDT_Simple) {
    auto input = getAnySignerInput3();
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    ASSERT_EQ(output.error(), Common::Proto::OK);
    // https://explorer.nervos.org/transaction/0x9b15f2bea26b98201540d8e20e8b1c21d96dd77ad246520b405c6aabb7173371
    ASSERT_EQ(output.transaction_id(),
              "0x9b15f2bea26b98201540d8e20e8b1c21d96dd77ad246520b405c6aabb7173371");
    ASSERT_EQ(
        output.transaction_json(),
        "{\"cell_deps\":[{\"dep_type\":\"dep_group\",\"out_point\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c\"}},{\"dep_type\":"
        "\"code\",\"out_point\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0xc7813f6a415144643970c2e88e0bb6ca6a8edc5dd7c1022746f628284a9936d5\"}}],\"header_deps\":"
        "[],\"inputs\":[{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0xe118bd11a73d381daf288381ce182d92b6cf2f52d25886bbda9e1a61525c7c4a\"},\"since\":\"0x0\"}"
        ",{\"previous_output\":{\"index\":\"0x1\",\"tx_hash\":"
        "\"0x09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a\"},\"since\":\"0x0\"}"
        ",{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0x5b12911e7413e011f251c1fb5fae4e76fd5fcae4f0d4c6412dcc5b0bfcece823\"},\"since\":\"0x0\"}"
        "],\"outputs\":[{\"capacity\":\"0x35a4e9000\",\"lock\":{\"args\":"
        "\"0xab201f55b02f53b385f79b34dfad548e549b48fc\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":{\"args\":"
        "\"0x9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8\",\"code_hash\":"
        "\"0x5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5\",\"hash_type\":"
        "\"type\"}},{\"capacity\":\"0x35a4e9000\",\"lock\":{\"args\":"
        "\"0xb0d65be39059d6489231b48f85ad706a560bbd8d\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":{\"args\":"
        "\"0x9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8\",\"code_hash\":"
        "\"0x5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5\",\"hash_type\":"
        "\"type\"}},{\"capacity\":\"0x1e95b03db\",\"lock\":{\"args\":"
        "\"0xb0d65be39059d6489231b48f85ad706a560bbd8d\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":null}],\"outputs_data\":[\"0x0080c6a47e8d03000000000000000000\","
        "\"0x00601e253b6b4c000000000000000000\",\"0x\"],\"version\":\"0x0\",\"witnesses\":["
        "\"0x550000001000000055000000550000004100000035d55fd46316f248552eb6af7ac9589c9ec533c4e5b718"
        "96b05cdf697e2d18551ceff54d7b860ebb2f4dd5f6c5bb4af1da15460a7621f5aa4bc7d5585a0504de00\","
        "\"0x5500000010000000550000005500000041000000eaa4bf69126d3016ab786610f2f0668b2ef353915d623d"
        "0b0184fc25cec3dcad6dc08a1504a2d7dd9faced17b041d79d4c21f1977e57859713360f5e3609583501\","
        "\"0x\"]}");
}

Proto::SigningInput getAnySignerInput4() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_sudt_transfer();

    operation.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02"
                             "wectaumxn0664yw2jd53lqk4mxg3");
    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama");
    operation.set_use_max_amount(true);
    input.set_byte_fee(1);
    auto sudtAddress =
        parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8");
    *operation.mutable_sudt_address() = std::string(sudtAddress.begin(), sudtAddress.end());

    auto& cell1 = *input.add_cell();
    cell1.set_capacity(8210026306);
    *cell1.mutable_out_point() =
        OutPoint(parse_hex("430cb60ee816e2631d6d9605659c18fec8eb3de94526f5fd4ad51feaad6f1664"), 0)
            .proto();
    *cell1.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();

    auto& cell2 = *input.add_cell();
    cell2.set_capacity(14400000000);
    *cell2.mutable_out_point() =
        OutPoint(parse_hex("378b6bd2f7fc2b1599ee55be7e8fa17fdd6e0d25e2e146d5f46006e0292d6564"), 0)
            .proto();
    *cell2.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();
    *cell2.mutable_type() =
        Script(parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5"),
               HashType::Type1,
               parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8"))
            .proto();
    auto cell2Data = parse_hex("00601e253b6b4c000000000000000000");
    *cell2.mutable_data() = std::string(cell2Data.begin(), cell2Data.end());

    auto privateKey = parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb");
    input.add_private_key(std::string(privateKey.begin(), privateKey.end()));

    return input;
}

TEST(TWAnySignerNervos, Sign_SUDT_SendMaximum) {
    auto input = getAnySignerInput4();
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    ASSERT_EQ(output.error(), Common::Proto::OK);
    // https://explorer.nervos.org/transaction/0x09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a
    ASSERT_EQ(output.transaction_id(),
              "0x09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a");
    ASSERT_EQ(output.transaction_json(),
              "{\"cell_deps\":[{\"dep_type\":\"dep_group\",\"out_point\":{\"index\":\"0x0\",\"tx_"
              "hash\":\"0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c\"}},{"
              "\"dep_type\":\"code\",\"out_point\":{\"index\":\"0x0\",\"tx_hash\":"
              "\"0xc7813f6a415144643970c2e88e0bb6ca6a8edc5dd7c1022746f628284a9936d5\"}}],\"header_"
              "deps\":[],\"inputs\":[{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
              "\"0x378b6bd2f7fc2b1599ee55be7e8fa17fdd6e0d25e2e146d5f46006e0292d6564\"},\"since\":"
              "\"0x0\"},{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
              "\"0x430cb60ee816e2631d6d9605659c18fec8eb3de94526f5fd4ad51feaad6f1664\"},\"since\":"
              "\"0x0\"}],\"outputs\":[{\"capacity\":\"0x35a4e9000\",\"lock\":{\"args\":"
              "\"0xab201f55b02f53b385f79b34dfad548e549b48fc\",\"code_hash\":"
              "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_"
              "type\":\"type\"},\"type\":{\"args\":"
              "\"0x9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8\",\"code_"
              "hash\":\"0x5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5\","
              "\"hash_type\":\"type\"}},{\"capacity\":\"0x1e95b0c5f\",\"lock\":{\"args\":"
              "\"0xb0d65be39059d6489231b48f85ad706a560bbd8d\",\"code_hash\":"
              "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_"
              "type\":\"type\"},\"type\":null}],\"outputs_data\":["
              "\"0x00601e253b6b4c000000000000000000\",\"0x\"],\"version\":\"0x0\",\"witnesses\":["
              "\"0x5500000010000000550000005500000041000000da7c908bdf2cb091b7ff9bb682b762d1323c5e1e"
              "cf9b2ce0ebedb9d55f6625c52ab14910ae401833112f2ea516ab11bc9ef691c3dff7886e3238c9348c3d"
              "73a701\",\"0x\"]}");
}

Proto::SigningInput getAnySignerInput5() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_dao_deposit();

    operation.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8f"
                             "urras980hksatlslfaktks7epf25");
    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9e"
                                 "rg8furras980hksatlslfaktks7epf25");
    operation.set_amount(10200000000);
    input.set_byte_fee(1);

    auto& cell1 = *input.add_cell();
    cell1.set_capacity(8210021909);
    *cell1.mutable_out_point() =
        OutPoint(parse_hex("c7dacd4aab49f5f9643e87752428cebde38eeb49c7726781c4d8b526822004a1"), 1)
            .proto();
    *cell1.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqds6ed78yze6eyfyvd537z66ur22c9mmrgz82ama"))
                                .proto();

    auto& cell2 = *input.add_cell();
    cell2.set_capacity(14399998167);
    *cell2.mutable_out_point() =
        OutPoint(parse_hex("d3c3263170815b326779e2fd8d548f846ae13eff9d9a82833c7071069a1d32bf"), 0)
            .proto();
    *cell2.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();

    auto privateKey1 =
        parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb");
    input.add_private_key(std::string(privateKey1.begin(), privateKey1.end()));
    auto privateKey2 =
        parse_hex("0c8859a9d9084a8c2b55963268b352e258756f9240f2a1f4645c610ed191dae9");
    input.add_private_key(std::string(privateKey2.begin(), privateKey2.end()));

    return input;
}

TEST(TWAnySignerNervos, Sign_DAO_Deposit) {
    auto input = getAnySignerInput5();
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    ASSERT_EQ(output.error(), Common::Proto::OK);
    // https://explorer.nervos.org/transaction/0x583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683
    ASSERT_EQ(output.transaction_id(),
              "0x583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683");
    ASSERT_EQ(
        output.transaction_json(),
        "{\"cell_deps\":[{\"dep_type\":\"dep_group\",\"out_point\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c\"}},{\"dep_type\":"
        "\"code\",\"out_point\":{\"index\":\"0x2\",\"tx_hash\":"
        "\"0xe2fb199810d49a4d8beec56718ba2593b665db9d52299a0f9e6e75416d73ff5c\"}}],\"header_deps\":"
        "[],\"inputs\":[{\"previous_output\":{\"index\":\"0x1\",\"tx_hash\":"
        "\"0xc7dacd4aab49f5f9643e87752428cebde38eeb49c7726781c4d8b526822004a1\"},\"since\":\"0x0\"}"
        ",{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0xd3c3263170815b326779e2fd8d548f846ae13eff9d9a82833c7071069a1d32bf\"},\"since\":\"0x0\"}"
        "],\"outputs\":[{\"capacity\":\"0x25ff7a600\",\"lock\":{\"args\":"
        "\"0xc4b50c5c8d074f063ec0a77ded0eaff0fa7b65da\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":{\"args\":\"0x\",\"code_hash\":"
        "\"0x82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e\",\"hash_type\":"
        "\"type\"}},{\"capacity\":\"0x2e3b1de31\",\"lock\":{\"args\":"
        "\"0xc4b50c5c8d074f063ec0a77ded0eaff0fa7b65da\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":null}],\"outputs_data\":[\"0x0000000000000000\",\"0x\"],\"version\":"
        "\"0x0\",\"witnesses\":["
        "\"0x5500000010000000550000005500000041000000305d09c7de3f34a4d53bc4e0031ee59c95b9abc4fc3ff5"
        "548e1a17ca726c069a232012c9c4be6ec4d4ffbe88613ca5e686e3e4b7d0b9bbd7038003e23ffdcdd601\","
        "\"0x55000000100000005500000055000000410000007c514c77482dd1e1086f41a6d17364c9b5ed16364d61df"
        "6f7fd8540f8bf7c131275c877943786b1b72fbf4f9d817ee5dd554a689808b7919543c691b5068e5be01\"]}");
}

Proto::SigningInput getAnySignerInput6() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_dao_withdraw_phase1();

    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9e"
                                 "rg8furras980hksatlslfaktks7epf25");
    auto& depositCell = *operation.mutable_deposit_cell();
    depositCell.set_capacity(10200000000);
    *depositCell.mutable_out_point() =
        OutPoint(parse_hex("583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683"), 0)
            .proto();
    *depositCell.mutable_lock() =
        Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8furras9"
                       "80hksatlslfaktks7epf25"))
            .proto();
    *depositCell.mutable_type() =
        Script(parse_hex("82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e"),
               HashType::Type1, Data())
            .proto();
    auto depositCellData = parse_hex("0000000000000000");
    *depositCell.mutable_data() = std::string(depositCellData.begin(), depositCellData.end());
    depositCell.set_block_number(7575466);
    auto blockHashData =
        parse_hex("3dfdb4b702a355a5593315016f8af0537d5a2f3292811b79420ded78a092be6a");
    *depositCell.mutable_block_hash() = std::string(blockHashData.begin(), blockHashData.end());
    input.set_byte_fee(1);

    auto& cell1 = *input.add_cell();
    cell1.set_capacity(10200000000);
    *cell1.mutable_out_point() =
        OutPoint(parse_hex("583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683"), 0)
            .proto();
    *cell1.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();
    *cell1.mutable_type() =
        Script(parse_hex("82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e"),
               HashType::Type1, Data())
            .proto();
    auto cell1Data = parse_hex("0000000000000000");
    *cell1.mutable_data() = std::string(cell1Data.begin(), cell1Data.end());

    auto& cell2 = *input.add_cell();
    cell2.set_capacity(12410019377);
    *cell2.mutable_out_point() =
        OutPoint(parse_hex("583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683"), 1)
            .proto();
    *cell2.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();

    auto privateKey1 =
        parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb");
    input.add_private_key(std::string(privateKey1.begin(), privateKey1.end()));

    return input;
}

TEST(TWAnySignerNervos, Sign_DAO_Withdraw_Phase1) {
    auto input = getAnySignerInput6();
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    ASSERT_EQ(output.error(), Common::Proto::OK);
    // https://explorer.nervos.org/transaction/0xb4e62bc5f5108275b0ef3da8f8cc3fb0172843c4a2a9cdfef3b04d6c65e9acca
    ASSERT_EQ(output.transaction_id(),
              "0xb4e62bc5f5108275b0ef3da8f8cc3fb0172843c4a2a9cdfef3b04d6c65e9acca");
    ASSERT_EQ(output.transaction_json(),
              "{\"cell_deps\":[{\"dep_type\":\"dep_group\",\"out_point\":{\"index\":\"0x0\",\"tx_"
              "hash\":\"0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c\"}},{"
              "\"dep_type\":\"code\",\"out_point\":{\"index\":\"0x2\",\"tx_hash\":"
              "\"0xe2fb199810d49a4d8beec56718ba2593b665db9d52299a0f9e6e75416d73ff5c\"}}],\"header_"
              "deps\":[\"0x3dfdb4b702a355a5593315016f8af0537d5a2f3292811b79420ded78a092be6a\"],"
              "\"inputs\":[{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
              "\"0x583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683\"},\"since\":"
              "\"0x0\"},{\"previous_output\":{\"index\":\"0x1\",\"tx_hash\":"
              "\"0x583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683\"},\"since\":"
              "\"0x0\"}],\"outputs\":[{\"capacity\":\"0x25ff7a600\",\"lock\":{\"args\":"
              "\"0xc4b50c5c8d074f063ec0a77ded0eaff0fa7b65da\",\"code_hash\":"
              "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_"
              "type\":\"type\"},\"type\":{\"args\":\"0x\",\"code_hash\":"
              "\"0x82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e\",\"hash_"
              "type\":\"type\"}},{\"capacity\":\"0x2e3b1db56\",\"lock\":{\"args\":"
              "\"0xc4b50c5c8d074f063ec0a77ded0eaff0fa7b65da\",\"code_hash\":"
              "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_"
              "type\":\"type\"},\"type\":null}],\"outputs_data\":[\"0xaa97730000000000\",\"0x\"],"
              "\"version\":\"0x0\",\"witnesses\":["
              "\"0x5500000010000000550000005500000041000000d5131c1a6b8eca11e2c280b72c5db09ea00bb788"
              "fd3262eaced861c39db2aad04a36f9d174b6f167a9c98b85d2bccf537a163c44459d23467dfa86408f48"
              "dd5f01\",\"0x\"]}");
}

Proto::SigningInput getAnySignerInput7() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_dao_withdraw_phase2();

    auto& depositCell = *operation.mutable_deposit_cell();
    depositCell.set_capacity(10200000000);
    *depositCell.mutable_out_point() =
        OutPoint(parse_hex("583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683"), 0)
            .proto();
    *depositCell.mutable_lock() =
        Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8furras9"
                       "80hksatlslfaktks7epf25"))
            .proto();
    *depositCell.mutable_type() =
        Script(parse_hex("82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e"),
               HashType::Type1, Data())
            .proto();
    auto depositCellData = parse_hex("0000000000000000");
    *depositCell.mutable_data() = std::string(depositCellData.begin(), depositCellData.end());
    depositCell.set_block_number(7575466);
    auto blockHashData1 =
        parse_hex("3dfdb4b702a355a5593315016f8af0537d5a2f3292811b79420ded78a092be6a");
    *depositCell.mutable_block_hash() = std::string(blockHashData1.begin(), blockHashData1.end());

    auto& withdrawingCell = *operation.mutable_withdrawing_cell();
    withdrawingCell.set_capacity(10200000000);
    *withdrawingCell.mutable_out_point() =
        OutPoint(parse_hex("b4e62bc5f5108275b0ef3da8f8cc3fb0172843c4a2a9cdfef3b04d6c65e9acca"), 0)
            .proto();
    *withdrawingCell.mutable_lock() =
        Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8furras9"
                       "80hksatlslfaktks7epf25"))
            .proto();
    *withdrawingCell.mutable_type() =
        Script(parse_hex("82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e"),
               HashType::Type1, Data())
            .proto();
    auto withdrawingCellData = parse_hex("aa97730000000000");
    *withdrawingCell.mutable_data() =
        std::string(withdrawingCellData.begin(), withdrawingCellData.end());
    withdrawingCell.set_block_number(7575534);
    auto blockHashData2 =
        parse_hex("b070d5364afd47c23fe267077d454009d6f665f200a915e68af1616e46f4aa52");
    *withdrawingCell.mutable_block_hash() =
        std::string(blockHashData2.begin(), blockHashData2.end());
    withdrawingCell.set_since(0x20037c0000001731);

    operation.set_amount(10200000000);
    input.set_byte_fee(1);

    auto& cell1 = *input.add_cell();
    cell1.set_capacity(10200000000);
    *cell1.mutable_out_point() =
        OutPoint(parse_hex("b4e62bc5f5108275b0ef3da8f8cc3fb0172843c4a2a9cdfef3b04d6c65e9acca"), 0)
            .proto();
    *cell1.mutable_lock() = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50x"
                                           "wsqwyk5x9erg8furras980hksatlslfaktks7epf25"))
                                .proto();
    *cell1.mutable_type() =
        Script(parse_hex("82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e"),
               HashType::Type1, Data())
            .proto();
    auto cell1Data = parse_hex("aa97730000000000");
    *cell1.mutable_data() = std::string(cell1Data.begin(), cell1Data.end());

    auto privateKey1 =
        parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb");
    input.add_private_key(std::string(privateKey1.begin(), privateKey1.end()));

    return input;
}

TEST(TWAnySignerNervos, Sign_DAO_Withdraw_Phase2) {
    auto input = getAnySignerInput7();
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    ASSERT_EQ(output.error(), Common::Proto::OK);
    ASSERT_EQ(output.transaction_id(),
              "0x4fde13c93fc5d24ab7f660070aaa0b1725809d585f6258605e595cdbd856ea1c");
    ASSERT_EQ(
        output.transaction_json(),
        "{\"cell_deps\":[{\"dep_type\":\"dep_group\",\"out_point\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c\"}},{\"dep_type\":"
        "\"code\",\"out_point\":{\"index\":\"0x2\",\"tx_hash\":"
        "\"0xe2fb199810d49a4d8beec56718ba2593b665db9d52299a0f9e6e75416d73ff5c\"}}],\"header_deps\":"
        "[\"0x3dfdb4b702a355a5593315016f8af0537d5a2f3292811b79420ded78a092be6a\","
        "\"0xb070d5364afd47c23fe267077d454009d6f665f200a915e68af1616e46f4aa52\"],\"inputs\":[{"
        "\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":"
        "\"0xb4e62bc5f5108275b0ef3da8f8cc3fb0172843c4a2a9cdfef3b04d6c65e9acca\"},\"since\":"
        "\"0x20037c0000001731\"}],\"outputs\":[{\"capacity\":\"0x25ff7a42c\",\"lock\":{\"args\":"
        "\"0xc4b50c5c8d074f063ec0a77ded0eaff0fa7b65da\",\"code_hash\":"
        "\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":"
        "\"type\"},\"type\":null}],\"outputs_data\":[\"0x\"],\"version\":\"0x0\",\"witnesses\":["
        "\"0x6100000010000000550000006100000041000000743f86c5557f4e2d3327f4d17e7bad27209b29c1e9cdba"
        "b42ab03f7094af917b4b203ddd7f2e87102e09ae579f2fe7f6adb7900b7386b58c1183ba0011b7c42100080000"
        "000000000000000000\"]}");
}

} // namespace TW::Nervos::tests