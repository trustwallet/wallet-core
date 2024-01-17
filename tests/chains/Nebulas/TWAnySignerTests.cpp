// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "uint256.h"
#include "proto/Nebulas.pb.h"

#include <gtest/gtest.h>

namespace TW::Nebulas::tests {

TEST(TWAnySignerNebulas, Sign) {
    Proto::SigningInput input;
    input.set_from_address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    input.set_to_address("n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17");
    auto value = store(uint256_t(7));
    input.set_nonce(value.data(), value.size());
    value = store(uint256_t(1000000));
    input.set_gas_price(value.data(), value.size());
    value = store(uint256_t(200000));
    input.set_gas_limit(value.data(), value.size());
    value = store(uint256_t(11000000000000000000ULL));
    input.set_amount(value.data(), value.size());
    input.set_payload("");
    value = store(uint256_t(1560052938));
    input.set_timestamp(value.data(), value.size());

    const auto privateKey = parse_hex("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b");
    input.set_private_key(privateKey.data(), privateKey.size());
    auto chainid = store(uint256_t(1));
    input.set_chain_id(chainid.data(), chainid.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNebulas);

    EXPECT_EQ(hex(output.signature()), "f53f4a9141ff8e462b094138eccd8c3a5d7865f9e9ab509626c78460a9e0b0fc35f7ed5ba1795ceb81a5e46b7580a6f7fb431d44fdba92515399cf6a8e47e71500");
    EXPECT_EQ(output.raw(), "CiBQXdR2neMqnEu21q/U+OHqZHSBX9Q0hNiRfL2eCZO4hRIaGVefwtw23wEobqA40/7aIwQHghETxH4r+50aGhlXf89CeLWgHFjKu9/6tn4KNbelsMDAIIi2IhAAAAAAAAAAAJin2bgxTAAAKAcwyony5wU6CAoGYmluYXJ5QAFKEAAAAAAAAAAAAAAAAAAPQkBSEAAAAAAAAAAAAAAAAAADDUBYAWJB9T9KkUH/jkYrCUE47M2MOl14Zfnpq1CWJseEYKngsPw19+1boXlc64Gl5Gt1gKb3+0MdRP26klFTmc9qjkfnFQA=");
}

} // namespace TW::Nebulas::tests
