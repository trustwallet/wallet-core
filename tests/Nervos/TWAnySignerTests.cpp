// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Nervos/Address.h"
#include "Nervos/Serialization.h"
#include "Nervos/Signer.h"
#include "Nervos/Transaction.h"
#include "Nervos/TransactionPlan.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Nervos.pb.h"
#include "uint256.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Nervos;

Proto::SigningInput getAnySignerInput1() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_native_transfer();

    operation.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02"
                             "wectaumxn0664yw2jd53lqk4mxg3");
    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama");
    auto amount = Serialization::encodeUint256(uint256_t(10000000000), 16);
    *input.mutable_amount() = std::string(amount.begin(), amount.end());
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

    ASSERT_EQ(output.transaction_id(),
              "f2c32afde7e72011985583873bc16c0a3c01fc01fc161eb4b914fcf84c53cdf8");

    auto txProto = output.transaction();

    ASSERT_EQ(txProto.cell_deps_size(), 1);

    auto cellDep1 = txProto.cell_deps(0);
    auto cellDep1TxHash =
        parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c");
    ASSERT_EQ(cellDep1.out_point().tx_hash(),
              std::string(cellDep1TxHash.begin(), cellDep1TxHash.end()));
    ASSERT_EQ(cellDep1.out_point().index(), 0);
    ASSERT_EQ(cellDep1.dep_type(), "dep_group");

    ASSERT_EQ(txProto.header_deps_size(), 0);

    ASSERT_EQ(txProto.inputs_size(), 1);

    auto cellInput1 = CellInput(txProto.inputs(0));
    ASSERT_EQ(cellInput1.previousOutput.txHash,
              parse_hex("71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3"));
    ASSERT_EQ(cellInput1.previousOutput.index, 0);
    ASSERT_EQ(cellInput1.since, 0);

    ASSERT_EQ(txProto.outputs_size(), 2);
    ASSERT_EQ(txProto.outputs_data_size(), 2);

    auto cellOutput1 = CellOutput(txProto.outputs(0));
    ASSERT_EQ(cellOutput1.capacity, 10000000000);
    ASSERT_EQ(cellOutput1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(cellOutput1.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput1.type.args.size(), 0);
    ASSERT_EQ(txProto.outputs_data(0).length(), 0);

    auto cellOutput2 = CellOutput(txProto.outputs(1));
    ASSERT_EQ(cellOutput2.capacity, 9999999536);
    ASSERT_EQ(cellOutput2.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput2.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput2.lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(cellOutput2.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput2.type.args.size(), 0);
    ASSERT_EQ(txProto.outputs_data(1).length(), 0);

    ASSERT_EQ(txProto.witnesses_size(), 1);
    ASSERT_EQ(
        hex(txProto.witnesses(0)),
        "55000000100000005500000055000000410000002a9ef2ad7829e5ea0c7a32735d29a0cb2ec20434f6fd5bf6e2"
        "9cda56b28e08140156191cbbf80313d3c9cae4b74607acce7b28eb21d52ef058ed8491cdde70b700");
}

void checkPlan1(Proto::TransactionPlan& txPlanProto) {
    ASSERT_EQ(txPlanProto.error(), Common::Proto::OK);

    ASSERT_EQ(txPlanProto.cell_deps_size(), 1);

    auto cellDep1 = txPlanProto.cell_deps(0);
    auto cellDep1TxHash =
        parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c");
    ASSERT_EQ(cellDep1.out_point().tx_hash(),
              std::string(cellDep1TxHash.begin(), cellDep1TxHash.end()));
    ASSERT_EQ(cellDep1.out_point().index(), 0);
    ASSERT_EQ(cellDep1.dep_type(), "dep_group");

    ASSERT_EQ(txPlanProto.header_deps_size(), 0);

    ASSERT_EQ(txPlanProto.selected_cells_size(), 1);

    auto cell1 = Cell(txPlanProto.selected_cells(0));
    ASSERT_EQ(cell1.outPoint.txHash,
              parse_hex("71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3"));
    ASSERT_EQ(cell1.outPoint.index, 0);
    ASSERT_EQ(cell1.capacity, 20000000000);
    ASSERT_EQ(cell1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cell1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cell1.lock.args, parse_hex("c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da"));
    ASSERT_EQ(cell1.type.codeHash.size(), 0);
    ASSERT_EQ(cell1.type.args.size(), 0);
    ASSERT_EQ(cell1.data.size(), 0);

    ASSERT_EQ(txPlanProto.outputs_size(), 2);
    ASSERT_EQ(txPlanProto.outputs_data_size(), 2);

    auto cellOutput1 = CellOutput(txPlanProto.outputs(0));
    ASSERT_EQ(cellOutput1.capacity, 10000000000);
    ASSERT_EQ(cellOutput1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(cellOutput1.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput1.type.args.size(), 0);
    ASSERT_EQ(txPlanProto.outputs_data(0).length(), 0);

    auto cellOutput2 = CellOutput(txPlanProto.outputs(1));
    ASSERT_EQ(cellOutput2.capacity, 9999999536);
    ASSERT_EQ(cellOutput2.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput2.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput2.lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(cellOutput2.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput2.type.args.size(), 0);
    ASSERT_EQ(txPlanProto.outputs_data(1).length(), 0);
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
    auto amount = Serialization::encodeUint256(uint256_t(1000000000000), 16);
    *input.mutable_amount() = std::string(amount.begin(), amount.end());
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
    input.set_use_max_amount(true);
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

    ASSERT_EQ(output.transaction_id(),
              "298f5e04b6900796614b89062eb96cec63c3b2c460d01058736a793b567bc5c8");

    auto txProto = output.transaction();

    ASSERT_EQ(txProto.cell_deps_size(), 1);

    auto cellDep1 = txProto.cell_deps(0);
    auto cellDep1TxHash =
        parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c");
    ASSERT_EQ(cellDep1.out_point().tx_hash(),
              std::string(cellDep1TxHash.begin(), cellDep1TxHash.end()));
    ASSERT_EQ(cellDep1.out_point().index(), 0);
    ASSERT_EQ(cellDep1.dep_type(), "dep_group");

    ASSERT_EQ(txProto.header_deps_size(), 0);

    ASSERT_EQ(txProto.inputs_size(), 1);

    auto cellInput1 = CellInput(txProto.inputs(0));
    ASSERT_EQ(cellInput1.previousOutput.txHash,
              parse_hex("c75567c80dc9b97aaf4e5c23f4c7f37b077f2b33a50dd7abd952abfbd5beb247"));
    ASSERT_EQ(cellInput1.previousOutput.index, 0);
    ASSERT_EQ(cellInput1.since, 0);

    ASSERT_EQ(txProto.outputs_size(), 1);
    ASSERT_EQ(txProto.outputs_data_size(), 1);

    auto cellOutput1 = CellOutput(txProto.outputs(0));
    ASSERT_EQ(cellOutput1.capacity, 11410040265);
    ASSERT_EQ(cellOutput1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(cellOutput1.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput1.type.args.size(), 0);
    ASSERT_EQ(txProto.outputs_data(0).length(), 0);

    ASSERT_EQ(txProto.witnesses_size(), 1);
    ASSERT_EQ(
        hex(txProto.witnesses(0)),
        "5500000010000000550000005500000041000000daf6e65e5a1fe447a4feb7199886b6635c44738e04ea594576"
        "08fb1c447e068026529d57b02014ddc144622f886153df426853f22083f8891461eeb50b5ce97d01");
}

Proto::SigningInput getAnySignerInput3() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_sudt_transfer();

    operation.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02"
                             "wectaumxn0664yw2jd53lqk4mxg3");
    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama");
    auto amount = Serialization::encodeUint256(uint256_t(1000000000000000), 16);
    *input.mutable_amount() = std::string(amount.begin(), amount.end());
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

    ASSERT_EQ(output.transaction_id(),
              "9b15f2bea26b98201540d8e20e8b1c21d96dd77ad246520b405c6aabb7173371");

    auto txProto = output.transaction();

    ASSERT_EQ(txProto.cell_deps_size(), 2);

    auto cellDep1 = txProto.cell_deps(0);
    auto cellDep1TxHash =
        parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c");
    ASSERT_EQ(cellDep1.out_point().tx_hash(),
              std::string(cellDep1TxHash.begin(), cellDep1TxHash.end()));
    ASSERT_EQ(cellDep1.out_point().index(), 0);
    ASSERT_EQ(cellDep1.dep_type(), "dep_group");

    auto cellDep2 = txProto.cell_deps(1);
    auto cellDep2TxHash =
        parse_hex("c7813f6a415144643970c2e88e0bb6ca6a8edc5dd7c1022746f628284a9936d5");
    ASSERT_EQ(cellDep2.out_point().tx_hash(),
              std::string(cellDep2TxHash.begin(), cellDep2TxHash.end()));
    ASSERT_EQ(cellDep2.out_point().index(), 0);
    ASSERT_EQ(cellDep2.dep_type(), "code");

    ASSERT_EQ(txProto.header_deps_size(), 0);

    ASSERT_EQ(txProto.inputs_size(), 3);

    auto cellInput1 = CellInput(txProto.inputs(0));
    ASSERT_EQ(cellInput1.previousOutput.txHash,
              parse_hex("e118bd11a73d381daf288381ce182d92b6cf2f52d25886bbda9e1a61525c7c4a"));
    ASSERT_EQ(cellInput1.previousOutput.index, 0);
    ASSERT_EQ(cellInput1.since, 0);

    auto cellInput2 = CellInput(txProto.inputs(1));
    ASSERT_EQ(cellInput2.previousOutput.txHash,
              parse_hex("09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a"));
    ASSERT_EQ(cellInput2.previousOutput.index, 1);
    ASSERT_EQ(cellInput2.since, 0);

    auto cellInput3 = CellInput(txProto.inputs(2));
    ASSERT_EQ(cellInput3.previousOutput.txHash,
              parse_hex("5b12911e7413e011f251c1fb5fae4e76fd5fcae4f0d4c6412dcc5b0bfcece823"));
    ASSERT_EQ(cellInput3.previousOutput.index, 0);
    ASSERT_EQ(cellInput3.since, 0);
    ASSERT_EQ(txProto.outputs_size(), 3);
    ASSERT_EQ(txProto.outputs_data_size(), 3);

    auto cellOutput1 = CellOutput(txProto.outputs(0));
    ASSERT_EQ(cellOutput1.capacity, 14400000000);
    ASSERT_EQ(cellOutput1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(cellOutput1.type.codeHash,
              parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5"));
    ASSERT_EQ(cellOutput1.type.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.type.args,
              parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8"));
    ASSERT_EQ(hex(txProto.outputs_data(0)), "0080c6a47e8d03000000000000000000");

    auto cellOutput2 = CellOutput(txProto.outputs(1));
    ASSERT_EQ(cellOutput2.capacity, 14400000000);
    ASSERT_EQ(cellOutput2.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput2.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput2.lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(cellOutput2.type.codeHash,
              parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5"));
    ASSERT_EQ(cellOutput2.type.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput2.type.args,
              parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8"));
    ASSERT_EQ(hex(txProto.outputs_data(1)), "00601e253b6b4c000000000000000000");

    auto cellOutput3 = CellOutput(txProto.outputs(2));
    ASSERT_EQ(cellOutput3.capacity, 8210023387);
    ASSERT_EQ(cellOutput3.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput3.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput3.lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(cellOutput3.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput3.type.args.size(), 0);
    ASSERT_EQ(txProto.outputs_data(2).length(), 0);

    ASSERT_EQ(txProto.witnesses_size(), 3);
    ASSERT_EQ(
        hex(txProto.witnesses(0)),
        "550000001000000055000000550000004100000035d55fd46316f248552eb6af7ac9589c9ec533c4e5b71896b0"
        "5cdf697e2d18551ceff54d7b860ebb2f4dd5f6c5bb4af1da15460a7621f5aa4bc7d5585a0504de00");
    ASSERT_EQ(
        hex(txProto.witnesses(1)),
        "5500000010000000550000005500000041000000eaa4bf69126d3016ab786610f2f0668b2ef353915d623d0b01"
        "84fc25cec3dcad6dc08a1504a2d7dd9faced17b041d79d4c21f1977e57859713360f5e3609583501");
    ASSERT_EQ(hex(txProto.witnesses(2)), "");
}

Proto::SigningInput getAnySignerInput4() {
    auto input = Proto::SigningInput();
    auto& operation = *input.mutable_sudt_transfer();

    operation.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02"
                             "wectaumxn0664yw2jd53lqk4mxg3");
    operation.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama");
    input.set_use_max_amount(true);
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

    ASSERT_EQ(output.transaction_id(),
              "09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a");

    auto txProto = output.transaction();

    ASSERT_EQ(txProto.cell_deps_size(), 2);

    auto cellDep1 = txProto.cell_deps(0);
    auto cellDep1TxHash =
        parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c");
    ASSERT_EQ(cellDep1.out_point().tx_hash(),
              std::string(cellDep1TxHash.begin(), cellDep1TxHash.end()));
    ASSERT_EQ(cellDep1.out_point().index(), 0);
    ASSERT_EQ(cellDep1.dep_type(), "dep_group");

    auto cellDep2 = txProto.cell_deps(1);
    auto cellDep2TxHash =
        parse_hex("c7813f6a415144643970c2e88e0bb6ca6a8edc5dd7c1022746f628284a9936d5");
    ASSERT_EQ(cellDep2.out_point().tx_hash(),
              std::string(cellDep2TxHash.begin(), cellDep2TxHash.end()));
    ASSERT_EQ(cellDep2.out_point().index(), 0);
    ASSERT_EQ(cellDep2.dep_type(), "code");

    ASSERT_EQ(txProto.header_deps_size(), 0);

    ASSERT_EQ(txProto.inputs_size(), 2);

    auto cellInput1 = CellInput(txProto.inputs(0));
    ASSERT_EQ(cellInput1.previousOutput.txHash,
              parse_hex("378b6bd2f7fc2b1599ee55be7e8fa17fdd6e0d25e2e146d5f46006e0292d6564"));
    ASSERT_EQ(cellInput1.previousOutput.index, 0);
    ASSERT_EQ(cellInput1.since, 0);

    auto cellInput2 = CellInput(txProto.inputs(1));
    ASSERT_EQ(cellInput2.previousOutput.txHash,
              parse_hex("430cb60ee816e2631d6d9605659c18fec8eb3de94526f5fd4ad51feaad6f1664"));
    ASSERT_EQ(cellInput2.previousOutput.index, 0);
    ASSERT_EQ(cellInput2.since, 0);

    ASSERT_EQ(txProto.outputs_size(), 2);
    ASSERT_EQ(txProto.outputs_data_size(), 2);

    auto cellOutput1 = CellOutput(txProto.outputs(0));
    ASSERT_EQ(cellOutput1.capacity, 14400000000);
    ASSERT_EQ(cellOutput1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(cellOutput1.type.codeHash,
              parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5"));
    ASSERT_EQ(cellOutput1.type.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.type.args,
              parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8"));
    ASSERT_EQ(hex(txProto.outputs_data(0)), "00601e253b6b4c000000000000000000");

    auto cellOutput2 = CellOutput(txProto.outputs(1));
    ASSERT_EQ(cellOutput2.capacity, 8210025567);
    ASSERT_EQ(cellOutput2.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput2.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput2.lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(cellOutput2.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput2.type.args.size(), 0);
    ASSERT_EQ(txProto.outputs_data(1).length(), 0);

    ASSERT_EQ(txProto.witnesses_size(), 2);
    ASSERT_EQ(
        hex(txProto.witnesses(0)),
        "5500000010000000550000005500000041000000da7c908bdf2cb091b7ff9bb682b762d1323c5e1ecf9b2ce0eb"
        "edb9d55f6625c52ab14910ae401833112f2ea516ab11bc9ef691c3dff7886e3238c9348c3d73a701");
    ASSERT_EQ(hex(txProto.witnesses(1)), "");
}
