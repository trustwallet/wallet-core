// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Nervos/Address.h"
#include "Nervos/Cell.h"
#include "Nervos/Serialization.h"
#include "Nervos/Signer.h"
#include "Nervos/Transaction.h"
#include "Nervos/TransactionPlan.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

namespace TW::Nervos::tests {

std::vector<PrivateKey> getPrivateKeys(Proto::SigningInput& input) {
    std::vector<PrivateKey> privateKeys;
    privateKeys.reserve(input.private_key_size());
    for (auto&& privateKey : input.private_key()) {
        privateKeys.emplace_back(privateKey);
    }
    return privateKeys;
}

Proto::SigningInput getInput1() {
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

void checkOutput1(Transaction& tx) {
    // https://explorer.nervos.org/transaction/0xf2c32afde7e72011985583873bc16c0a3c01fc01fc161eb4b914fcf84c53cdf8
    ASSERT_EQ(tx.hash(),
              parse_hex("f2c32afde7e72011985583873bc16c0a3c01fc01fc161eb4b914fcf84c53cdf8"));

    ASSERT_EQ(tx.cellDeps.size(), 1ul);

    ASSERT_EQ(tx.cellDeps[0].outPoint.txHash,
              parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"));
    ASSERT_EQ(tx.cellDeps[0].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[0].depType, DepType::DepGroup);

    ASSERT_EQ(tx.headerDeps.size(), 0ul);

    ASSERT_EQ(tx.inputs.size(), 1ul);

    ASSERT_EQ(tx.inputs[0].previousOutput.txHash,
              parse_hex("71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3"));
    ASSERT_EQ(tx.inputs[0].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[0].since, 0ul);

    ASSERT_EQ(tx.outputs.size(), 2ul);
    ASSERT_EQ(tx.outputsData.size(), 2ul);

    ASSERT_EQ(tx.outputs[0].capacity, 10000000000ul);
    ASSERT_EQ(tx.outputs[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(tx.outputs[0].type.codeHash.size(), 0ul);
    ASSERT_EQ(tx.outputs[0].type.args.size(), 0ul);
    ASSERT_EQ(tx.outputsData[0].size(), 0ul);

    ASSERT_EQ(tx.outputs[1].capacity, 9999999536ul);
    ASSERT_EQ(tx.outputs[1].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[1].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[1].lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(tx.outputs[1].type.codeHash.size(), 0ul);
    ASSERT_EQ(tx.outputs[1].type.args.size(), 0ul);
    ASSERT_EQ(tx.outputsData[1].size(), 0ul);

    ASSERT_EQ(tx.serializedWitnesses.size(), 1ul);
    ASSERT_EQ(
        hex(tx.serializedWitnesses[0]),
        "55000000100000005500000055000000410000002a9ef2ad7829e5ea0c7a32735d29a0cb2ec20434f6fd5bf6e2"
        "9cda56b28e08140156191cbbf80313d3c9cae4b74607acce7b28eb21d52ef058ed8491cdde70b700");
}

void checkPlan1(TransactionPlan& txPlan) {
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);

    ASSERT_EQ(txPlan.cellDeps.size(), 1ul);

    ASSERT_EQ(txPlan.cellDeps[0].outPoint.txHash,
              parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"));
    ASSERT_EQ(txPlan.cellDeps[0].outPoint.index, 0ul);
    ASSERT_EQ(txPlan.cellDeps[0].depType, DepType::DepGroup);

    ASSERT_EQ(txPlan.headerDeps.size(), 0ul);

    ASSERT_EQ(txPlan.selectedCells.size(), 1ul);

    ASSERT_EQ(txPlan.selectedCells[0].outPoint.txHash,
              parse_hex("71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3"));
    ASSERT_EQ(txPlan.selectedCells[0].outPoint.index, 0ul);
    ASSERT_EQ(txPlan.selectedCells[0].capacity, 20000000000ul);
    ASSERT_EQ(txPlan.selectedCells[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(txPlan.selectedCells[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(txPlan.selectedCells[0].lock.args,
              parse_hex("c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da"));
    ASSERT_EQ(txPlan.selectedCells[0].type.codeHash.size(), 0ul);
    ASSERT_EQ(txPlan.selectedCells[0].type.args.size(), 0ul);
    ASSERT_EQ(txPlan.selectedCells[0].data.size(), 0ul);

    ASSERT_EQ(txPlan.outputs.size(), 2ul);
    ASSERT_EQ(txPlan.outputsData.size(), 2ul);

    ASSERT_EQ(txPlan.outputs[0].capacity, 10000000000ul);
    ASSERT_EQ(txPlan.outputs[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(txPlan.outputs[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(txPlan.outputs[0].lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(txPlan.outputs[0].type.codeHash.size(), 0ul);
    ASSERT_EQ(txPlan.outputs[0].type.args.size(), 0ul);
    ASSERT_EQ(txPlan.outputsData[0].size(), 0ul);

    ASSERT_EQ(txPlan.outputs[1].capacity, 9999999536ul);
    ASSERT_EQ(txPlan.outputs[1].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(txPlan.outputs[1].lock.hashType, HashType::Type1);
    ASSERT_EQ(txPlan.outputs[1].lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(txPlan.outputs[1].type.codeHash.size(), 0ul);
    ASSERT_EQ(txPlan.outputs[1].type.args.size(), 0ul);
    ASSERT_EQ(txPlan.outputsData[1].size(), 0ul);
}

TEST(NervosSigner, PlanAndSign_Native_Simple) {
    auto input = getInput1();
    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);
    checkPlan1(txPlan);
    Transaction tx;
    tx.build(txPlan);
    auto error = tx.sign(getPrivateKeys(input));
    ASSERT_EQ(error, Common::Proto::SigningError::OK);
    checkOutput1(tx);
}

TEST(NervosSigner, Sign_NegativeMissingKey) {
    auto input = getInput1();
    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);
    Transaction tx;
    tx.build(txPlan);
    auto privateKey = parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61fec");
    *input.mutable_private_key(0) = std::string(privateKey.begin(), privateKey.end());
    auto error = tx.sign(getPrivateKeys(input));
    ASSERT_EQ(error, Common::Proto::Error_missing_private_key);
}

TEST(NervosSigner, Sign_NegativeNotEnoughUtxos) {
    auto input = getInput1();
    auto& operation = *input.mutable_native_transfer();
    operation.set_amount(1000000000000);
    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::Error_not_enough_utxos);
}

Proto::SigningInput getInput2() {
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

TEST(NervosSigner, Sign_Native_SendMaximum) {
    auto input = getInput2();
    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);
    Transaction tx;
    tx.build(txPlan);
    auto error = tx.sign(getPrivateKeys(input));

    ASSERT_EQ(error, Common::Proto::SigningError::OK);

    // https://explorer.nervos.org/transaction/0x298f5e04b6900796614b89062eb96cec63c3b2c460d01058736a793b567bc5c8
    ASSERT_EQ(tx.hash(),
              parse_hex("298f5e04b6900796614b89062eb96cec63c3b2c460d01058736a793b567bc5c8"));

    ASSERT_EQ(tx.cellDeps.size(), 1ul);

    ASSERT_EQ(tx.cellDeps[0].outPoint.txHash,
              parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"));
    ASSERT_EQ(tx.cellDeps[0].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[0].depType, DepType::DepGroup);

    ASSERT_EQ(tx.headerDeps.size(), 0ul);

    ASSERT_EQ(tx.inputs.size(), 1ul);

    ASSERT_EQ(tx.inputs[0].previousOutput.txHash,
              parse_hex("c75567c80dc9b97aaf4e5c23f4c7f37b077f2b33a50dd7abd952abfbd5beb247"));
    ASSERT_EQ(tx.inputs[0].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[0].since, 0ul);

    ASSERT_EQ(tx.outputs.size(), 1ul);
    ASSERT_EQ(tx.outputsData.size(), 1ul);

    ASSERT_EQ(tx.outputs[0].capacity, 11410040265ul);
    ASSERT_EQ(tx.outputs[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(tx.outputs[0].type.codeHash.size(), 0ul);
    ASSERT_EQ(tx.outputs[0].type.args.size(), 0ul);
    ASSERT_EQ(tx.outputsData[0].size(), 0ul);

    ASSERT_EQ(tx.serializedWitnesses.size(), 1ul);
    ASSERT_EQ(
        hex(tx.serializedWitnesses[0]),
        "5500000010000000550000005500000041000000daf6e65e5a1fe447a4feb7199886b6635c44738e04ea594576"
        "08fb1c447e068026529d57b02014ddc144622f886153df426853f22083f8891461eeb50b5ce97d01");
}

Proto::SigningInput getInput3() {
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

TEST(NervosSigner, Sign_SUDT_Simple) {
    auto input = getInput3();

    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);
    Transaction tx;
    tx.build(txPlan);
    auto error = tx.sign(getPrivateKeys(input));

    ASSERT_EQ(error, Common::Proto::SigningError::OK);

    // https://explorer.nervos.org/transaction/0x9b15f2bea26b98201540d8e20e8b1c21d96dd77ad246520b405c6aabb7173371
    ASSERT_EQ(tx.hash(),
              parse_hex("9b15f2bea26b98201540d8e20e8b1c21d96dd77ad246520b405c6aabb7173371"));

    ASSERT_EQ(tx.cellDeps.size(), 2ul);

    ASSERT_EQ(tx.cellDeps[0].outPoint.txHash,
              parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"));
    ASSERT_EQ(tx.cellDeps[0].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[0].depType, DepType::DepGroup);

    ASSERT_EQ(tx.cellDeps[1].outPoint.txHash,
              parse_hex("c7813f6a415144643970c2e88e0bb6ca6a8edc5dd7c1022746f628284a9936d5"));
    ASSERT_EQ(tx.cellDeps[1].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[1].depType, DepType::Code);

    ASSERT_EQ(tx.headerDeps.size(), 0ul);

    ASSERT_EQ(tx.inputs.size(), 3ul);

    ASSERT_EQ(tx.inputs[0].previousOutput.txHash,
              parse_hex("e118bd11a73d381daf288381ce182d92b6cf2f52d25886bbda9e1a61525c7c4a"));
    ASSERT_EQ(tx.inputs[0].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[0].since, 0ul);

    ASSERT_EQ(tx.inputs[1].previousOutput.txHash,
              parse_hex("09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a"));
    ASSERT_EQ(tx.inputs[1].previousOutput.index, 1ul);
    ASSERT_EQ(tx.inputs[1].since, 0ul);

    ASSERT_EQ(tx.inputs[2].previousOutput.txHash,
              parse_hex("5b12911e7413e011f251c1fb5fae4e76fd5fcae4f0d4c6412dcc5b0bfcece823"));
    ASSERT_EQ(tx.inputs[2].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[2].since, 0ul);

    ASSERT_EQ(tx.outputs.size(), 3ul);
    ASSERT_EQ(tx.outputsData.size(), 3ul);

    ASSERT_EQ(tx.outputs[0].capacity, 14400000000ul);
    ASSERT_EQ(tx.outputs[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(tx.outputs[0].type.codeHash,
              parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5"));
    ASSERT_EQ(tx.outputs[0].type.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].type.args,
              parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8"));
    ASSERT_EQ(hex(tx.outputsData[0]), "0080c6a47e8d03000000000000000000");

    ASSERT_EQ(tx.outputs[1].capacity, 14400000000ul);
    ASSERT_EQ(tx.outputs[1].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[1].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[1].lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(tx.outputs[1].type.codeHash,
              parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5"));
    ASSERT_EQ(tx.outputs[1].type.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[1].type.args,
              parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8"));
    ASSERT_EQ(hex(tx.outputsData[1]), "00601e253b6b4c000000000000000000");

    ASSERT_EQ(tx.outputs[2].capacity, 8210023387ul);
    ASSERT_EQ(tx.outputs[2].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[2].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[2].lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(tx.outputs[2].type.codeHash.size(), 0ul);
    ASSERT_EQ(tx.outputs[2].type.args.size(), 0ul);
    ASSERT_EQ(tx.outputsData[2].size(), 0ul);

    ASSERT_EQ(tx.serializedWitnesses.size(), 3ul);
    ASSERT_EQ(
        hex(tx.serializedWitnesses[0]),
        "550000001000000055000000550000004100000035d55fd46316f248552eb6af7ac9589c9ec533c4e5b71896b0"
        "5cdf697e2d18551ceff54d7b860ebb2f4dd5f6c5bb4af1da15460a7621f5aa4bc7d5585a0504de00");
    ASSERT_EQ(
        hex(tx.serializedWitnesses[1]),
        "5500000010000000550000005500000041000000eaa4bf69126d3016ab786610f2f0668b2ef353915d623d0b01"
        "84fc25cec3dcad6dc08a1504a2d7dd9faced17b041d79d4c21f1977e57859713360f5e3609583501");
    ASSERT_EQ(hex(tx.serializedWitnesses[2]), "");
}

Proto::SigningInput getInput4() {
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

TEST(NervosSigner, Sign_SUDT_SendMaximum) {
    auto input = getInput4();
    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);
    Transaction tx;
    tx.build(txPlan);
    auto error = tx.sign(getPrivateKeys(input));

    ASSERT_EQ(error, Common::Proto::SigningError::OK);

    // https://explorer.nervos.org/transaction/0x09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a
    ASSERT_EQ(tx.hash(),
              parse_hex("09a45a15e48f985b554a0b6e5f0857913cc492ec061cc9b0b2befa4b24609a4a"));

    ASSERT_EQ(tx.cellDeps.size(), 2ul);

    ASSERT_EQ(tx.cellDeps[0].outPoint.txHash,
              parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"));
    ASSERT_EQ(tx.cellDeps[0].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[0].depType, DepType::DepGroup);

    ASSERT_EQ(tx.cellDeps[1].outPoint.txHash,
              parse_hex("c7813f6a415144643970c2e88e0bb6ca6a8edc5dd7c1022746f628284a9936d5"));
    ASSERT_EQ(tx.cellDeps[1].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[1].depType, DepType::Code);

    ASSERT_EQ(tx.headerDeps.size(), 0ul);

    ASSERT_EQ(tx.inputs.size(), 2ul);

    ASSERT_EQ(tx.inputs[0].previousOutput.txHash,
              parse_hex("378b6bd2f7fc2b1599ee55be7e8fa17fdd6e0d25e2e146d5f46006e0292d6564"));
    ASSERT_EQ(tx.inputs[0].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[0].since, 0ul);

    ASSERT_EQ(tx.inputs[1].previousOutput.txHash,
              parse_hex("430cb60ee816e2631d6d9605659c18fec8eb3de94526f5fd4ad51feaad6f1664"));
    ASSERT_EQ(tx.inputs[1].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[1].since, 0ul);

    ASSERT_EQ(tx.outputs.size(), 2ul);
    ASSERT_EQ(tx.outputsData.size(), 2ul);

    ASSERT_EQ(tx.outputs[0].capacity, 14400000000ul);
    ASSERT_EQ(tx.outputs[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(tx.outputs[0].type.codeHash,
              parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5"));
    ASSERT_EQ(tx.outputs[0].type.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].type.args,
              parse_hex("9657b32fcdc463e13ec9205914fd91c443822a949937ae94add9869e7f2e1de8"));
    ASSERT_EQ(hex(tx.outputsData[0]), "00601e253b6b4c000000000000000000");

    ASSERT_EQ(tx.outputs[1].capacity, 8210025567ul);
    ASSERT_EQ(tx.outputs[1].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[1].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[1].lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(tx.outputs[1].type.codeHash.size(), 0ul);
    ASSERT_EQ(tx.outputs[1].type.args.size(), 0ul);
    ASSERT_EQ(tx.outputsData[1].size(), 0ul);

    ASSERT_EQ(tx.serializedWitnesses.size(), 2ul);
    ASSERT_EQ(
        hex(tx.serializedWitnesses[0]),
        "5500000010000000550000005500000041000000da7c908bdf2cb091b7ff9bb682b762d1323c5e1ecf9b2ce0eb"
        "edb9d55f6625c52ab14910ae401833112f2ea516ab11bc9ef691c3dff7886e3238c9348c3d73a701");
    ASSERT_EQ(hex(tx.serializedWitnesses[1]), "");
}

Proto::SigningInput getInput5() {
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

TEST(NervosSigner, Sign_DAO_Deposit) {
    auto input = getInput5();

    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);
    Transaction tx;
    tx.build(txPlan);
    auto error = tx.sign(getPrivateKeys(input));

    ASSERT_EQ(error, Common::Proto::SigningError::OK);

    // https://explorer.nervos.org/transaction/0x583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683
    ASSERT_EQ(tx.hash(),
              parse_hex("583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683"));

    ASSERT_EQ(tx.cellDeps.size(), 2ul);

    ASSERT_EQ(tx.cellDeps[0].outPoint.txHash,
              parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"));
    ASSERT_EQ(tx.cellDeps[0].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[0].depType, DepType::DepGroup);

    ASSERT_EQ(tx.cellDeps[1].outPoint.txHash,
              parse_hex("e2fb199810d49a4d8beec56718ba2593b665db9d52299a0f9e6e75416d73ff5c"));
    ASSERT_EQ(tx.cellDeps[1].outPoint.index, 2ul);
    ASSERT_EQ(tx.cellDeps[1].depType, DepType::Code);

    ASSERT_EQ(tx.headerDeps.size(), 0ul);

    ASSERT_EQ(tx.inputs.size(), 2ul);

    ASSERT_EQ(tx.inputs[0].previousOutput.txHash,
              parse_hex("c7dacd4aab49f5f9643e87752428cebde38eeb49c7726781c4d8b526822004a1"));
    ASSERT_EQ(tx.inputs[0].previousOutput.index, 1ul);
    ASSERT_EQ(tx.inputs[0].since, 0ul);

    ASSERT_EQ(tx.inputs[1].previousOutput.txHash,
              parse_hex("d3c3263170815b326779e2fd8d548f846ae13eff9d9a82833c7071069a1d32bf"));
    ASSERT_EQ(tx.inputs[1].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[1].since, 0ul);

    ASSERT_EQ(tx.outputs.size(), 2ul);
    ASSERT_EQ(tx.outputsData.size(), 2ul);

    ASSERT_EQ(tx.outputs[0].capacity, 10200000000ul);
    ASSERT_EQ(tx.outputs[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].lock.args, parse_hex("c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da"));
    ASSERT_EQ(tx.outputs[0].type.codeHash,
              parse_hex("82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e"));
    ASSERT_EQ(tx.outputs[0].type.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].type.args, parse_hex(""));
    ASSERT_EQ(hex(tx.outputsData[0]), "0000000000000000");

    ASSERT_EQ(tx.outputs[1].capacity, 12410019377ul);
    ASSERT_EQ(tx.outputs[1].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[1].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[1].lock.args, parse_hex("c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da"));
    ASSERT_EQ(tx.outputs[1].type.codeHash.size(), 0ul);
    ASSERT_EQ(tx.outputs[1].type.args.size(), 0ul);
    ASSERT_EQ(tx.outputsData[1].size(), 0ul);

    ASSERT_EQ(tx.serializedWitnesses.size(), 2ul);
    ASSERT_EQ(
        hex(tx.serializedWitnesses[0]),
        "5500000010000000550000005500000041000000305d09c7de3f34a4d53bc4e0031ee59c95b9abc4fc3ff5548e"
        "1a17ca726c069a232012c9c4be6ec4d4ffbe88613ca5e686e3e4b7d0b9bbd7038003e23ffdcdd601");
    ASSERT_EQ(
        hex(tx.serializedWitnesses[1]),
        "55000000100000005500000055000000410000007c514c77482dd1e1086f41a6d17364c9b5ed16364d61df6f7f"
        "d8540f8bf7c131275c877943786b1b72fbf4f9d817ee5dd554a689808b7919543c691b5068e5be01");
}

Proto::SigningInput getInput6() {
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

TEST(NervosSigner, Sign_DAO_Withdraw_Phase1) {
    auto input = getInput6();

    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);
    Transaction tx;
    tx.build(txPlan);
    auto error = tx.sign(getPrivateKeys(input));

    ASSERT_EQ(error, Common::Proto::SigningError::OK);

    // https://explorer.nervos.org/transaction/0xb4e62bc5f5108275b0ef3da8f8cc3fb0172843c4a2a9cdfef3b04d6c65e9acca
    ASSERT_EQ(tx.hash(),
              parse_hex("b4e62bc5f5108275b0ef3da8f8cc3fb0172843c4a2a9cdfef3b04d6c65e9acca"));

    ASSERT_EQ(tx.cellDeps.size(), 2ul);

    ASSERT_EQ(tx.cellDeps[0].outPoint.txHash,
              parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"));
    ASSERT_EQ(tx.cellDeps[0].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[0].depType, DepType::DepGroup);

    ASSERT_EQ(tx.cellDeps[1].outPoint.txHash,
              parse_hex("e2fb199810d49a4d8beec56718ba2593b665db9d52299a0f9e6e75416d73ff5c"));
    ASSERT_EQ(tx.cellDeps[1].outPoint.index, 2ul);
    ASSERT_EQ(tx.cellDeps[1].depType, DepType::Code);

    ASSERT_EQ(tx.headerDeps.size(), 1ul);
    ASSERT_EQ(tx.headerDeps[0],
              parse_hex("3dfdb4b702a355a5593315016f8af0537d5a2f3292811b79420ded78a092be6a"));

    ASSERT_EQ(tx.inputs.size(), 2ul);

    ASSERT_EQ(tx.inputs[0].previousOutput.txHash,
              parse_hex("583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683"));
    ASSERT_EQ(tx.inputs[0].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[0].since, 0ul);

    ASSERT_EQ(tx.inputs[1].previousOutput.txHash,
              parse_hex("583d77a037e86155b7ab79ac59fc9bb01640e2427e859467ea10c4a6f222b683"));
    ASSERT_EQ(tx.inputs[1].previousOutput.index, 1ul);
    ASSERT_EQ(tx.inputs[1].since, 0ul);

    ASSERT_EQ(tx.outputs.size(), 2ul);
    ASSERT_EQ(tx.outputsData.size(), 2ul);

    ASSERT_EQ(tx.outputs[0].capacity, 10200000000ul);
    ASSERT_EQ(tx.outputs[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].lock.args, parse_hex("c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da"));
    ASSERT_EQ(tx.outputs[0].type.codeHash,
              parse_hex("82d76d1b75fe2fd9a27dfbaa65a039221a380d76c926f378d3f81cf3e7e13f2e"));
    ASSERT_EQ(tx.outputs[0].type.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].type.args, parse_hex(""));
    ASSERT_EQ(hex(tx.outputsData[0]), "aa97730000000000");

    ASSERT_EQ(tx.outputs[1].capacity, 12410018646ul);
    ASSERT_EQ(tx.outputs[1].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[1].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[1].lock.args, parse_hex("c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da"));
    ASSERT_EQ(tx.outputs[1].type.codeHash.size(), 0ul);
    ASSERT_EQ(tx.outputs[1].type.args.size(), 0ul);
    ASSERT_EQ(tx.outputsData[1].size(), 0ul);

    ASSERT_EQ(tx.serializedWitnesses.size(), 2ul);
    ASSERT_EQ(
        hex(tx.serializedWitnesses[0]),
        "5500000010000000550000005500000041000000d5131c1a6b8eca11e2c280b72c5db09ea00bb788fd3262eace"
        "d861c39db2aad04a36f9d174b6f167a9c98b85d2bccf537a163c44459d23467dfa86408f48dd5f01");
    ASSERT_EQ(tx.serializedWitnesses[1].size(), 0ul);
}

Proto::SigningInput getInput7() {
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

TEST(NervosSigner, Sign_DAO_Withdraw_Phase2) {
    auto input = getInput7();

    TransactionPlan txPlan;
    txPlan.plan(input);
    ASSERT_EQ(txPlan.error, Common::Proto::SigningError::OK);
    Transaction tx;
    tx.build(txPlan);
    auto error = tx.sign(getPrivateKeys(input));

    ASSERT_EQ(error, Common::Proto::SigningError::OK);

    ASSERT_EQ(tx.hash(),
              parse_hex("4fde13c93fc5d24ab7f660070aaa0b1725809d585f6258605e595cdbd856ea1c"));

    ASSERT_EQ(tx.cellDeps.size(), 2ul);

    ASSERT_EQ(tx.cellDeps[0].outPoint.txHash,
              parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"));
    ASSERT_EQ(tx.cellDeps[0].outPoint.index, 0ul);
    ASSERT_EQ(tx.cellDeps[0].depType, DepType::DepGroup);

    ASSERT_EQ(tx.cellDeps[1].outPoint.txHash,
              parse_hex("e2fb199810d49a4d8beec56718ba2593b665db9d52299a0f9e6e75416d73ff5c"));
    ASSERT_EQ(tx.cellDeps[1].outPoint.index, 2ul);
    ASSERT_EQ(tx.cellDeps[1].depType, DepType::Code);

    ASSERT_EQ(tx.headerDeps.size(), 2ul);
    ASSERT_EQ(tx.headerDeps[0],
              parse_hex("3dfdb4b702a355a5593315016f8af0537d5a2f3292811b79420ded78a092be6a"));
    ASSERT_EQ(tx.headerDeps[1],
              parse_hex("b070d5364afd47c23fe267077d454009d6f665f200a915e68af1616e46f4aa52"));

    ASSERT_EQ(tx.inputs.size(), 1ul);

    ASSERT_EQ(tx.inputs[0].previousOutput.txHash,
              parse_hex("b4e62bc5f5108275b0ef3da8f8cc3fb0172843c4a2a9cdfef3b04d6c65e9acca"));
    ASSERT_EQ(tx.inputs[0].previousOutput.index, 0ul);
    ASSERT_EQ(tx.inputs[0].since, 0x20037c0000001731ul);

    ASSERT_EQ(tx.outputs.size(), 1ul);
    ASSERT_EQ(tx.outputsData.size(), 1ul);

    ASSERT_EQ(tx.outputs[0].capacity, 10199999532ul);
    ASSERT_EQ(tx.outputs[0].lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(tx.outputs[0].lock.hashType, HashType::Type1);
    ASSERT_EQ(tx.outputs[0].lock.args, parse_hex("c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da"));
    ASSERT_EQ(tx.outputs[0].type.codeHash.size(), 0ul);
    ASSERT_EQ(tx.outputs[0].type.args.size(), 0ul);
    ASSERT_EQ(tx.outputsData[0].size(), 0ul);

    ASSERT_EQ(tx.serializedWitnesses.size(), 1ul);
    ASSERT_EQ(hex(tx.serializedWitnesses[0]),
              "6100000010000000550000006100000041000000743f86c5557f4e2d3327f4d17e7bad27209b29c1e9cd"
              "bab42ab03f7094af917b4b203ddd7f2e87102e09ae579f2fe7f6adb7900b7386b58c1183ba0011b7c421"
              "00080000000000000000000000");
}

} // namespace TW::Nervos::tests
