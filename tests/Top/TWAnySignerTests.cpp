// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Top.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Top;

TEST(TWAnySignerTop, Sign) {
    auto privateKey = parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62");
    
    Proto::SigningInput input;
    input.set_from("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    input.set_to("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    input.set_amount(1);
    input.set_tx_deposit(2);
    input.set_last_tx_nonce(3);
    input.set_last_tx_hash("12345678");
    input.set_token("hello world");
    input.set_note("top unit test");
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTop);

    ASSERT_EQ(hex(output.encoded()), "f8d49466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a399003250102038831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374b8820488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bf216da6f9276fb23e718ef5c85bdcc8b63d5f88540b1b68978837e35334beb60d1de3efbe6948bb10636f9f014553b8b0ca5697a66b0c69c9dc0ce893289fce5100");
}
