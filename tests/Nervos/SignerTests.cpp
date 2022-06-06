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

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Nervos;

TEST(NervosSigner, Sign) {
    auto signingInput = SigningInput();
    signingInput.toAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqfq0njqh7wm5"
                             "69txsf85n5du4hvqwsqmdsl7zz3s";
    signingInput.changeAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama";
    signingInput.amount = 10000000000;
    signingInput.byteFee = 1;
    auto cell1 = Cell();
    cell1.outPoint.txHash =
        parse_hex("b8a5e232c78cd25c1af9b0c00f93867ac7b2a18312c1b0a0e8311a5dcca250e4");
    cell1.outPoint.index = 0;
    cell1.capacity = 20000000000;
    cell1.lock = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25"));
    signingInput.cells.push_back(cell1);
    signingInput.privateKeys.push_back(
        PrivateKey(parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb")));
    auto tx = Transaction();
    ASSERT_EQ(tx.plan(signingInput), Common::Proto::OK);
    ASSERT_EQ(tx.sign(signingInput.privateKeys), Common::Proto::OK);
    ASSERT_EQ(hex(tx.hash()), "d575198d229071f32774b8d718c2f40a68db837919973d88bfa89f441c180173");
    ASSERT_EQ(tx.witnesses.size(), 1);
    ASSERT_EQ(
        hex(tx.witnesses[0]),
        "55000000100000005500000055000000410000006725678fc2eaab0f2b6c1c026df876e7f35d0fa5eef5750a64"
        "ced4d382479d481093ff74989853052efad84b379f5925110b49cd249fc97ad5618b3a7df7405101");
}

TEST(NervosSigner, Sign_NegativeMissingKey) {
    auto signingInput = SigningInput();
    signingInput.toAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqfq0njqh7wm5"
                             "69txsf85n5du4hvqwsqmdsl7zz3s";
    signingInput.changeAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama";
    signingInput.amount = 10000000000;
    signingInput.byteFee = 1;
    auto cell1 = Cell();
    cell1.outPoint.txHash =
        parse_hex("b8a5e232c78cd25c1af9b0c00f93867ac7b2a18312c1b0a0e8311a5dcca250e4");
    cell1.outPoint.index = 0;
    cell1.capacity = 20000000000;
    cell1.lock = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25"));
    signingInput.cells.push_back(cell1);
    signingInput.privateKeys.push_back(
        PrivateKey(parse_hex("c0c999a33ac2faa0de55685c4b90b4304374620f10c1b6371fddc052722a9463")));
    auto tx = Transaction();
    ASSERT_EQ(tx.plan(signingInput), Common::Proto::OK);
    ASSERT_EQ(tx.sign(signingInput.privateKeys), Common::Proto::Error_missing_private_key);
}

TEST(NervosSigner, Sign_NegativeNotEnoughUtxos) {
    auto signingInput = SigningInput();
    signingInput.toAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqfq0njqh7wm5"
                             "69txsf85n5du4hvqwsqmdsl7zz3s";
    signingInput.changeAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78"
                                 "yze6eyfyvd537z66ur22c9mmrgz82ama";
    signingInput.amount = 10000000000;
    signingInput.byteFee = 1;
    auto cell1 = Cell();
    cell1.outPoint.txHash =
        parse_hex("b8a5e232c78cd25c1af9b0c00f93867ac7b2a18312c1b0a0e8311a5dcca250e4");
    cell1.outPoint.index = 0;
    cell1.capacity = 10000000000;
    cell1.lock = Script(Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25"));
    signingInput.cells.push_back(cell1);
    signingInput.privateKeys.push_back(
        PrivateKey(parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb")));
    auto tx = Transaction();
    ASSERT_EQ(tx.plan(signingInput), Common::Proto::Error_not_enough_utxos);
}
