// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "TestUtilities.h"
#include "proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <gtest/gtest.h>

namespace TW::Zclassic {

TEST(TWAnySignerZclassic, SignBasic) {
    // Same private key / UTXO as Zcash test, but using ZCL coin type
    // Private key corresponds to address t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz (ZCL)
    auto privateKey = parse_hex("a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559");
    auto txId = parse_hex("3a19dd44032dfed61bfca5ba5751aab8a107b30609cbd5d70dc5ef09885b6853");
    std::reverse(txId.begin(), txId.end());

    Bitcoin::Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(488000);
    input.set_byte_fee(1);
    input.set_to_address("t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS");
    input.set_change_address("t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz");
    input.set_coin_type(TWCoinTypeZclassic);

    auto utxo = input.add_utxo();
    utxo->mutable_out_point()->set_hash(txId.data(), txId.size());
    utxo->mutable_out_point()->set_index(0);
    utxo->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo->set_amount(494000);
    // P2PKH script for t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz
    auto utxoScript = parse_hex("76a914f84c7f4dd3c3dc311676444fdead6e6d290d50e388ac");
    utxo->set_script(utxoScript.data(), utxoScript.size());

    input.add_private_key(privateKey.data(), privateKey.size());

    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeZclassic);
    EXPECT_EQ(output.error(), Common::Proto::OK);
    EXPECT_GT(output.encoded().size(), 0);
}

} // namespace TW::Zclassic
