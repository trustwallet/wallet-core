// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NewChain/Signer.h"
#include "NewChain/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Ethereum/Transaction.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::NewChain;
using TransactionNonTyped = TW::Ethereum::TransactionNonTyped;


TEST(NewChainSigner, Sign) {
    const auto address = parse_hex("0x524e6cc2bfa3b2d3f35818ab47a3fcd5621a4ff9");
    const auto transaction = TransactionNonTyped::buildNativeTransfer(
        /* nonce: */ 1,
        /* gasPrice: */ 1,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000l
    );

    const auto key = PrivateKey(parse_hex("298b9bee0a0431e8f1a81323df6810b72467db21f9c252cb6d134d149005a386"));
    const auto signature = Signer::sign(key, 1642, transaction);
    std::cout << "hello" << std::endl;
    std::cout << signature.r << std::endl;
    std::cout << signature.s << std::endl;
    ASSERT_EQ(signature.v, 3319);
    ASSERT_EQ(signature.r, uint256_t("46571765288188216591534782960426097699348716487079705089053857077177255770603"));
    ASSERT_EQ(signature.s, uint256_t("12308270257649765253016795538144573553021596290578393318018091052042357355987"));
}
