// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonyChainID.h>
#include <gtest/gtest.h>

#include "Ethereum/RLP.h"
#include "Harmony-One/Address.h"
#include "Harmony-One/Signer.h"
#include "HexCoding.h"

namespace TW::Harmony {

using namespace boost::multiprecision;

class SignerExposed : public Signer {
  public:
    SignerExposed(uint256_t chainID) : Signer(chainID) {}
    using Signer::hash;
};

TEST(HarmonySigner, RLPEncoding) {
    auto address = Address("0x587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2");
    auto rlp_unhashed_should_be = "ec0880825208010194587c66b4b973a7b231d02ebbc7e"
                                  "7d9f6c5a49ef2890168d28e3f00280000808201908080";
    uint256_t amount("0x56bc75e2d63100000");
    auto transaction = Transaction(
        /* nonce: */ 0x8,
        /* gasPrice: */ 0x0,
        /* gasLimit: */ 0x5208,
        /* fromShardID */ 0x1,
        /* toShardID */ 0x1,
        /* to: */ address,
        /* amount: */ uint256_t("0x168d28e3f00280000"),
        /* payload: */ {});
    auto signer = SignerExposed(uint256_t("400"));
    auto rlp_hex = signer.txn_as_rlp_hex(transaction);
    ASSERT_EQ(rlp_hex, rlp_unhashed_should_be);
}

TEST(HarmonySigner, Hash) {
    auto address = Address("0x587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2");
    auto should_be = "3092f57f57a88d9e822947b1b6245d19"
                     "5600e698b4c829044e7553256a52498c";
    uint256_t amount("0x56bc75e2d63100000");
    auto transaction = Transaction(
        /* nonce: */ 0x9,
        /* gasPrice: */ 0x0,
        /* gasLimit: */ 0x5208,
        /* fromShardID */ 0x3,
        /* toShardID */ 0x2,
        /* to: */ address,
        /* amount: */ uint256_t("0x168d28e3f00280000"),
        /* payload: */ {});
    auto signer = SignerExposed(uint256_t("400"));
    auto hash = signer.hash(transaction);
    ASSERT_EQ(hex(hash), should_be);
}

TEST(HarmonySigner, Sign) {
    auto address = Address("0x587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2");
    auto transaction = Transaction(
        /* nonce: */ 0x9,
        /* gasPrice: */ 0x0,
        /* gasLimit: */ 0x5208,
        /* fromShardID */ 0x3,
        /* toShardID */ 0x2,
        /* to: */ address,
        /* amount: */ uint256_t("0x168d28e3f00280000"),
        /* payload: */ {});
    auto key =
        PrivateKey(parse_hex("0xe2f88b4974ae763ca1c2db49218802c2e441293a09eaa9ab681779e05d1b7b94"));
    auto signer = SignerExposed(TWHarmonyChainIDLocalNet);
    uint256_t should_be_r("0xf4757c9ffad127996f788fb388be3e3e03440f6980b36dc6cee7230e390f0c13");
    uint256_t should_be_s("0x42f0ff332bd552e8ad7a1cf6a0af4ebebfb1f8aae413c54d3464b9babba5f28d");
    signer.sign(key, transaction);
    ASSERT_EQ(transaction.v, 835);
    ASSERT_EQ(transaction.r, should_be_r);
    ASSERT_EQ(transaction.s, should_be_s);
}

} // namespace TW::Harmony
