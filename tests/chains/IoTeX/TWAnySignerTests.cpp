// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "proto/IoTeX.pb.h"

#include <gtest/gtest.h>

namespace TW::IoTeX::tests {

TEST(TWAnySignerIoTeX, Sign) {
    auto key = parse_hex("68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748");
    Proto::SigningInput input;
    input.set_version(1);
    input.set_nonce(1);
    input.set_gaslimit(1);
    input.set_gasprice("1");
    input.set_privatekey(key.data(), key.size());
    auto& transfer = *input.mutable_transfer();
    transfer.set_amount("1");
    transfer.set_recipient("io1e2nqsyt7fkpzs5x7zf2uk0jj72teu5n6aku3tr");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeIoTeX);

    ASSERT_EQ(hex(output.encoded()), "0a39080110011801220131522e0a01311229696f3165326e7173797437666b707a733578377a6632756b306a6a3732746575356e36616b75337472124104fb30b196ce3e976593ecc2da220dca9cdea8c84d2373770042a930b892ac0f5cf762f20459c9100eb9d4d7597f5817bf21e10b53a0120b9ec1ba5cddfdcb669b1a41ec9757ae6c9009315830faaab250b6db0e9535b00843277f596ae0b2b9efc0bd4e14138c056fc4cdfa285d13dd618052b3d1cb7a3f554722005a2941bfede96601");
}

} // namespace TW::IoTeX::tests
