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

TEST(ZilliqaSigner, Signing) {
    auto privateKey =
        PrivateKey(parse_hex("0E891B9DFF485000C7D1DC22ECF3A583CC50328684321D61947A86E57CF6C638"));
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(pubKey.bytes), "034ae47910d58b9bde819c3cffa8de4441955508db00aa2540db8e6bf6e99abc1b");
    
    auto amount = uint256_t(15000000000000);
    auto gasPrice = uint256_t(1000000000);
    auto amountData = store(amount);
    auto gasData = store(gasPrice);

    auto input = Proto::SigningInput();
    input.set_version(65537);
    input.set_nonce(3);
    input.set_to_address("0x9Ca91EB535Fb92Fda5094110FDaEB752eDb9B039");
    input.set_amount(amountData.data(), amountData.size());
    input.set_gas_price(gasData.data(), gasData.size());
    input.set_gas_limit(uint64_t(1));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto output = Signer::sign(input);
    auto signature = output.signature();

    ASSERT_EQ(hex(signature.begin(), signature.end()), "1727048b9f200d07e6e2d8eff6ba3e54bf491589ccb55d872e242505b72e6ca74e022ac954df21420af8737c89a7e9048b75b531f6865b78b1e2a864169ed64d");
}
