// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "TestUtilities.h"
#include "Zcash/Transaction.h"

#include "proto/Bitcoin.pb.h"
#include "proto/BitcoinV2.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWCoinType.h>

#include <vector>
#include <iterator>
#include <cassert>
#include <gtest/gtest.h>

namespace TW::Zcash {

// Tests the BitcoinV2 API through the legacy `SigningInput`.
// Successfully broadcasted: https://explorer.zcha.in/transactions/ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256
TEST(TWAnySignerZcash, SignSapplingV2) {
    auto privateKey = parse_hex("a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559");
    auto txId = parse_hex("3a19dd44032dfed61bfca5ba5751aab8a107b30609cbd5d70dc5ef09885b6853");
    std::reverse(txId.begin(), txId.end());
    int64_t inAmount = 494'000;
    int64_t outAmount = 488'000;
    auto senderAddress = "t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz";
    auto toAddress = "t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS";

    BitcoinV2::Proto::SigningInput signing;
    signing.add_private_keys(privateKey.data(), privateKey.size());

    auto& chainInfo = *signing.mutable_chain_info();
    chainInfo.set_p2pkh_prefix(184);
    chainInfo.set_p2sh_prefix(189);

    auto& builder = *signing.mutable_builder();
    builder.set_version(BitcoinV2::Proto::TransactionVersion::UseDefault);
    builder.set_input_selector(BitcoinV2::Proto::InputSelector::UseAll);
    builder.set_fixed_dust_threshold(546);

    auto& extraData = *builder.mutable_zcash_extra_data();
    extraData.set_branch_id(SaplingBranchID.data(), SaplingBranchID.size());

    auto& in = *builder.add_inputs();
    auto& inOutPoint = *in.mutable_out_point();
    inOutPoint.set_hash(txId.data(), txId.size());
    inOutPoint.set_vout(0);
    in.set_value(inAmount);
    in.set_receiver_address(senderAddress);
    in.set_sighash_type(TWBitcoinSigHashTypeAll);

    auto& out = *builder.add_outputs();
    out.set_value(outAmount);
    out.set_to_address(toAddress);

    Bitcoin::Proto::SigningInput legacy;
    legacy.set_coin_type(TWCoinTypeZcash);
    *legacy.mutable_signing_v2() = signing;

    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(legacy, TWCoinTypeZcash);

    EXPECT_EQ(output.error(), Common::Proto::OK);
    ASSERT_TRUE(output.has_signing_result_v2());
    EXPECT_EQ(output.signing_result_v2().error(), Common::Proto::SigningError::OK)
                    << output.signing_result_v2().error_message();
    EXPECT_EQ(hex(output.signing_result_v2().encoded()), "0400008085202f890153685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a000000006b483045022100ca0be9f37a4975432a52bb65b25e483f6f93d577955290bb7fb0060a93bfc92002203e0627dff004d3c72a957dc9f8e4e0e696e69d125e4d8e275d119001924d3b48012103b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6ffffffff0140720700000000001976a91449964a736f3713d64283fd0018626ba50091c7e988ac00000000000000000000000000000000000000");
    EXPECT_EQ(hex(output.signing_result_v2().txid()), "ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256");
}

}
