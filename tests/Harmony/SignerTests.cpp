// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Harmony/Address.h"
#include "Harmony/RLP.h"
#include "Harmony/Signer.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Harmony {

using boost::multiprecision::uint256_t;

class SignerExposed : public Signer {
public:
    SignerExposed(boost::multiprecision::uint256_t chainID) : Signer(chainID) {}
    using Signer::hash;
};

// Harmony test data
    TEST(Signer, HarmonyHash) {
        auto address = Address("0xd7ff41ca29306122185a07d04293ddb35f24cf2d");
        auto transaction = Transaction(
                                       /* nonce: */ 0x0,
                                       /* gasPrice: */ 20000000000,
                                       /* gasLimit: */ 21000,
                                       /* to: */ address,
                                       ///* amount: */ 0x1158e460913d00000,
                                       //Err: integer literal is too large to be represented in any integer type
                                       
                                       /* amount: */ 0x58e460913d,
                                       
                                       /* payload: */ {}
                                       );
        auto signer = SignerExposed(1);
        auto hash = signer.hash(transaction);
        
        ASSERT_EQ(hex(hash), "dff358dad4d0fc95b11acc9826b190d8b7971ac26b3f7ebdee83c10cafaf86f");
    }

    
TEST(Signer, Hash) {
    auto address = Address("0x3535353535353535353535353535353535353535");
    auto transaction = Transaction(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000,
        /* payload: */ {}
    );
    auto signer = SignerExposed(1);
    auto hash = signer.hash(transaction);
    
    ASSERT_EQ(hex(hash), "daf5a779ae972f972197303d7b574746c7ef83eadac0f2791ad23db92e4c8e53");
}

    
    
TEST(Signer, Sign) {
    auto address = Address("0x3535353535353535353535353535353535353535");
    auto transaction = Transaction(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000,
        /* payload: */ {}
    );

    auto key = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    auto signer = SignerExposed(1);
    signer.sign(key, transaction);

    ASSERT_EQ(transaction.v, 37);
    ASSERT_EQ(transaction.r, uint256_t("18515461264373351373200002665853028612451056578545711640558177340181847433846"));
    ASSERT_EQ(transaction.s, uint256_t("46948507304638947509940763649030358759909902576025900602547168820602576006531"));
}

} // namespace TW::Harmony
