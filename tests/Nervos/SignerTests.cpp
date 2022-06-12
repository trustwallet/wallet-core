// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Nervos/Address.h"
#include "Nervos/Signer.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Nervos.pb.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Nervos;

Proto::SigningInput getInput1() {
    auto input = Proto::SigningInput();
    input.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdty"
                         "q04tvp02wectaumxn0664yw2jd53lqk4mxg3");
    input.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xws"
                             "qds6ed78yze6eyfyvd537z66ur22c9mmrgz82ama");
    input.set_amount(10000000000);
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

Proto::SigningInput getInput2() {
    auto input = Proto::SigningInput();
    input.set_to_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdty"
                         "q04tvp02wectaumxn0664yw2jd53lqk4mxg3");
    input.set_change_address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xws"
                             "qds6ed78yze6eyfyvd537z66ur22c9mmrgz82ama");
    input.set_amount(0);
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

TEST(NervosSigner, Sign_Simple) {
    auto input = getInput1();
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
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
    auto cellOutput1 = CellOutput(txProto.outputs(0));
    ASSERT_EQ(cellOutput1.capacity, 10000000000);
    ASSERT_EQ(cellOutput1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(cellOutput1.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput1.type.args.size(), 0);
    auto cellOutput2 = CellOutput(txProto.outputs(1));
    ASSERT_EQ(cellOutput2.capacity, 9999999536);
    ASSERT_EQ(cellOutput2.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput2.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput2.lock.args, parse_hex("b0d65be39059d6489231b48f85ad706a560bbd8d"));
    ASSERT_EQ(cellOutput2.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput2.type.args.size(), 0);
    ASSERT_EQ(txProto.outputs_data_size(), 2);
    ASSERT_EQ(txProto.outputs_data(0).length(), 0);
    ASSERT_EQ(txProto.outputs_data(1).length(), 0);
    ASSERT_EQ(txProto.witnesses_size(), 1);
    ASSERT_EQ(
        hex(txProto.witnesses(0)),
        "55000000100000005500000055000000410000002a9ef2ad7829e5ea0c7a32735d29a0cb2ec20434f6fd5bf6e2"
        "9cda56b28e08140156191cbbf80313d3c9cae4b74607acce7b28eb21d52ef058ed8491cdde70b700");
}

TEST(NervosSigner, Sign_SendMaximum) {
    auto input = getInput2();
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
    auto cellOutput1 = CellOutput(txProto.outputs(0));
    ASSERT_EQ(cellOutput1.capacity, 11410040265);
    ASSERT_EQ(cellOutput1.lock.codeHash,
              parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"));
    ASSERT_EQ(cellOutput1.lock.hashType, HashType::Type1);
    ASSERT_EQ(cellOutput1.lock.args, parse_hex("ab201f55b02f53b385f79b34dfad548e549b48fc"));
    ASSERT_EQ(cellOutput1.type.codeHash.size(), 0);
    ASSERT_EQ(cellOutput1.type.args.size(), 0);
    ASSERT_EQ(txProto.outputs_data_size(), 1);
    ASSERT_EQ(txProto.outputs_data(0).length(), 0);
    ASSERT_EQ(txProto.witnesses_size(), 1);
    ASSERT_EQ(
        hex(txProto.witnesses(0)),
        "5500000010000000550000005500000041000000daf6e65e5a1fe447a4feb7199886b6635c44738e04ea594576"
        "08fb1c447e068026529d57b02014ddc144622f886153df426853f22083f8891461eeb50b5ce97d01");
}

TEST(NervosSigner, Sign_NegativeMissingKey) {
    auto input = getInput1();
    input.clear_private_key();
    auto privateKey = parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61fec");
    input.add_private_key(std::string(privateKey.begin(), privateKey.end()));
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    ASSERT_EQ(output.error(), Common::Proto::Error_missing_private_key);
}

TEST(NervosSigner, Sign_NegativeNotEnoughUtxos) {
    auto input = getInput1();
    input.set_amount(1000000000000);
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNervos);
    ASSERT_EQ(output.error(), Common::Proto::Error_not_enough_utxos);
}
