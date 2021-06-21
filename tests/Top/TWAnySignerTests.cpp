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
    Data data = parse_hex("01020304");
    input.set_last_tx_hash(std::string((char*)data.data(), data.size()));
    data = parse_hex("05060708");
    input.set_token(std::string((char*)data.data(), data.size()));
    data = parse_hex("090a0b0c");
    input.set_note(std::string((char*)data.data(), data.size()));
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTop);

    ASSERT_EQ(hex(output.encoded()), "f83c9466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a399003250102038401020304840506070884090a0b0c");
}
