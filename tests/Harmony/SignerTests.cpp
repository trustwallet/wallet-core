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
// using boost::multiprecision::cpp_int;
// using boost::multiprecision::uint256_t;

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
    auto should_be = "c845";
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

    // auto signer = SignerExposed(uint256_t("400"));
    // auto hash = signer.hash(transaction);
    // ASSERT_EQ(hex(hash), should_be);
}

TEST(HarmonySigner, Sign) {
    auto address = Address("0x587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2");
    auto transaction = Transaction(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* fromShardID */ 1,
        /* toShardID */ 1,
        /* to: */ address,
        /* amount: */ 1000000000000000000,
        /* payload: */ {});
    auto key =
        PrivateKey(parse_hex("0xe2f88b4974ae763ca1c2db49218802c2e441293a09eaa9ab681779e05d1b7b94"));
    auto signer = SignerExposed(TWHarmonyChainIDLocalNet);
    signer.sign(key, transaction);
    // ASSERT_EQ(transaction.v, 37);
    // ASSERT_EQ(
    //     transaction.r,
    //     uint256_t("18515461264373351373200002665853028612451056578545711640558177340181847433846"));
    // ASSERT_EQ(transaction.s, uint256_t(""));
}

} // namespace TW::Harmony
