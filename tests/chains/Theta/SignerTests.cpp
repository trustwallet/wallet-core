// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Theta/Signer.h"

#include <gtest/gtest.h>

namespace TW::Theta {

using boost::multiprecision::uint256_t;

TEST(Signer, Sign) {
    const auto pkFrom =
        PrivateKey(parse_hex("0x93a90ea508331dfdf27fb79757d4250b4e84954927ba0073cd67454ac432c737"));
    const auto from = Ethereum::Address("0x2E833968E5bB786Ae419c4d13189fB081Cc43bab");
    const auto to = Ethereum::Address("0x9F1233798E905E173560071255140b4A8aBd3Ec6");
    auto transaction = Transaction(from, to, 10, 20, 1);

    auto signer = Signer("privatenet");
    auto signature = signer.sign(pkFrom, transaction);
    transaction.setSignature(from, signature);

    ASSERT_EQ(hex(signature), "5190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8"
                              "fe9134ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501");
    ASSERT_EQ(hex(transaction.encodePayload()),
              "02f887c78085e8d4a51000f863f861942e833968e5bb786ae419c4d13189fb081cc43babc70a85e8d4a5"
              "101401b8415190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134"
              "ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501d9d8949f1233798e905e17356007"
              "1255140b4a8abd3ec6c20a14");
}

} // namespace TW::Theta
