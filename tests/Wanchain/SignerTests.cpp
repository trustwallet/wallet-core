// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/Address.h"
#include "Ethereum/RLP.h"
#include "HexCoding.h"
#include "Wanchain/Signer.h"

#include <gtest/gtest.h>

namespace TW::Wanchain {

using boost::multiprecision::uint256_t;

class SignerExposed : public Signer {
public:
    SignerExposed(boost::multiprecision::uint256_t chainID) : Signer(chainID) {}
};

TEST(Signer, Sign) {
    auto address = parse_hex("0x3535353535353535353535353535353535353535");
    auto transaction = Transaction(Ethereum::TransactionLegacy::buildNativeTransfer(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000
    ));

    auto key = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    auto signer = SignerExposed(1);
    auto signature = signer.sign(key, transaction);
    //auto encoded = signer.encode(transaction);
    auto encoded = transaction.encoded(signature);

    ASSERT_EQ(hex(encoded.begin(), encoded.end()), "f86d01098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0b3efa52659bb3dfcc1356942a5024ca55d742d8a2b551a142c5a0427c11c6272a07f616e839104c8c9c8f83254afc4e5c7010293cd5e471f6756629d17c8392467");
    ASSERT_EQ(signature.v, 37);
    ASSERT_EQ(signature.r, uint256_t("81387416166961732074688826827458851755389597777750181844865627349674117325426"));
    ASSERT_EQ(signature.s, uint256_t("57615878674481745412864351256912693004505281517367788339541976444424345429095"));
}

} // namespace TW::Wanchain
