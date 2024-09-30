// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "TestUtilities.h"
#include "TrustWalletCore/TWAnySigner.h"

#include <gtest/gtest.h>

namespace TW::Cosmos::nativeInjective::tests {

TEST(NativeInjectiveSigner, Sign) {
   auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(17396);
    input.set_chain_id("injective-1");
    input.set_sequence(1);

    Address fromAddress;
    Address toAddress;
    EXPECT_TRUE(Address::decode("inj13u6g7vqgw074mgmf2ze2cadzvkz9snlwcrtq8a", fromAddress));
    EXPECT_TRUE(Address::decode("inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("inj");
    amountOfTx->set_amount("10000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(110000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("inj");
    amountOfFee->set_amount("100000000000000");

    auto privateKey = parse_hex("9ee18daf8e463877aaf497282abc216852420101430482a28e246c179e2c5ef1");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Cosmos::Proto::SigningOutput();
    ANY_SIGN(input, TWCoinTypeNativeInjective);

    // https://www.mintscan.io/injective/txs/135DD2C4A1910E4334A9C0F15125DA992E724EBF23FEB9638FCB71218BB064A5
    assertJSONEqual(output.serialized(), "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Co8BCowBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmwKKmluajEzdTZnN3ZxZ3cwNzRtZ21mMnplMmNhZHp2a3o5c25sd2NydHE4YRIqaW5qMXhtcGtteHI0YXMwMGVtMjN0YzJ6Z211eXkyZ3I0aDN3Z2NsNnZkGhIKA2luahILMTAwMDAwMDAwMDASngEKfgp0Ci0vaW5qZWN0aXZlLmNyeXB0by52MWJldGExLmV0aHNlY3AyNTZrMS5QdWJLZXkSQwpBBFoMa4O4vZgn5QcnDK20mbfjqQlSRvaiITKB94PYd8mLJWdCdBsGOfMXdo/k9MJ2JmDCESKDp2hdgVUH3uMikXMSBAoCCAEYARIcChYKA2luahIPMTAwMDAwMDAwMDAwMDAwELDbBhpAx2vkplmzeK7n3puCFGPWhLd0l/ZC/CYkGl+stH+3S3hiCvIe7uwwMpUlNaSwvT8HwF1kNUp+Sx2m0Uo1x5xcFw==\"}");
}

}
