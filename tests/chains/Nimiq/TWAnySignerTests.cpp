// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "proto/Nimiq.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Nimiq::tests {

TEST(TWAnySignerNimiq, Sign) {
    auto privateKey = parse_hex("e3cc33575834add098f8487123cd4bca543ee859b3e8cfe624e7e6a97202b756");
    
    Proto::SigningInput input;

    input.set_destination("NQ86 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0MLA");
    input.set_fee(1000);
    input.set_value(42042042);
    input.set_validity_start_height(314159);
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNimiq);

    EXPECT_EQ(hex(output.encoded()), "0070c7492aaa9c9ac7a05bc0d9c5db2dae9372029654f71f0c7f95deed5099b7021450ffc385cd4e7c6ac9a7e91614ca67ff90568a00000000028182ba00000000000003e80004cb2f2a74dc7f6e0ab58a0bf52cc6e8801b0cca132dd4229d9a3e3a3d2f90e4d8f045d981b771bf5fc3851a98f3c617b1a943228f963e910e061808a721cfa0e3cad50b");
}

TEST(TWAnySignerNimiq, SignPoS) {
    auto privateKey = parse_hex("e3cc33575834add098f8487123cd4bca543ee859b3e8cfe624e7e6a97202b756");

    Proto::SigningInput input;

    input.set_destination("NQ86 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0MLA");
    input.set_fee(1000);
    input.set_value(42042042);
    input.set_validity_start_height(314159);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_network_id(Proto::NetworkId::MainnetAlbatross);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNimiq);

    EXPECT_EQ(hex(output.encoded()), "000070c7492aaa9c9ac7a05bc0d9c5db2dae9372029654f71f0c7f95deed5099b7021450ffc385cd4e7c6ac9a7e91614ca67ff90568a00000000028182ba00000000000003e80004cb2f180ba678744be3bf9cd44fbcdabfb5be209f21739934836e26055610ab02720fa99489219d9f3581664473a1b40b30ad1f6e13150d59f8234a42c3f0de3d505405");
}

} // namespace TW::Nimiq::tests
