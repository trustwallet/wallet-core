// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Zilliqa/Address.h"
#include "Zilliqa/Signer.h"
#include "proto/Zilliqa.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Zilliqa;

TEST(ZilliqaSigner, PreImage) {
    auto privateKey = PrivateKey(parse_hex("0E891B9DFF485000C7D1DC22ECF3A583CC50328684321D61947A86E57CF6C638"));
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(pubKey.bytes), "034ae47910d58b9bde819c3cffa8de4441955508db00aa2540db8e6bf6e99abc1b");

    auto amount = uint256_t(15000000000000);
    auto gasPrice = uint256_t(1000000000);
    auto amountData = store(amount);
    auto gasData = store(gasPrice);

    auto input = Proto::SigningInput();
    input.set_version(65537);
    input.set_nonce(4);
    input.set_to_address("0x9Ca91EB535Fb92Fda5094110FDaEB752eDb9B039");
    input.set_amount(amountData.data(), amountData.size());
    input.set_gas_price(gasData.data(), gasData.size());
    input.set_gas_limit(uint64_t(1));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto preImage = Signer::getPreImage(input);
    auto signature = Signer::sign(preImage, privateKey).signature();

    ASSERT_EQ(hex(preImage.begin(), preImage.end()), "0881800410041a149ca91eb535fb92fda5094110fdaeb752edb9b03922230a21034ae47910d58b9bde819c3cffa8de4441955508db00aa2540db8e6bf6e99abc1b2a120a10000000000000000000000da475abf00032120a100000000000000000000000003b9aca003801");

    ASSERT_TRUE(pubKey.verifySchnorr(Data(signature.begin(), signature.end()), preImage));
}

TEST(ZilliqaSigner, Signing) {
    auto privateKey = PrivateKey(parse_hex("0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748"));
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    // 1 ZIL
    auto amount = uint256_t(1000000000000);
    auto gasPrice = uint256_t(1000000000);
    auto amountData = store(amount);
    auto gasData = store(gasPrice);

    auto input = Proto::SigningInput();
    input.set_version(65537);
    input.set_nonce(2);
    input.set_to_address("0x7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C");
    input.set_amount(amountData.data(), amountData.size());
    input.set_gas_price(gasData.data(), gasData.size());
    input.set_gas_limit(uint64_t(1));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto preImage = Signer::getPreImage(input);
    auto signature = Signer::sign(preImage, privateKey).signature();

    ASSERT_EQ(hex(signature.begin(), signature.end()), "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268");
}
