// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base64.h"
#include "Base58.h"
#include "HexCoding.h"
#include "proto/NEAR.pb.h"
#include "NEAR/Signer.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::NEAR {

TEST(NEARSigner, SignTx) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto& transfer = *input.mutable_actions(0)->mutable_transfer();
    Data deposit(16, 0);
    deposit[0] = 1;
    // uint128_t / little endian byte order
    transfer.set_deposit(deposit.data(), deposit.size());

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto output = Signer::sign(std::move(input));

    auto signed_transaction = output.signed_transaction();
    auto outputInBase64 = Base64::encode(Data(signed_transaction.begin(), signed_transaction.end()));

    ASSERT_EQ(outputInBase64, "CQAAAHRlc3QubmVhcgCRez0mjUtY9/7BsVC9aNab4+5dTMOYVeNBU4Rlu3eGDQEAAAAAAAAADQAAAHdoYXRldmVyLm5lYXIPpHP9JpAd8pa+atxMxN800EDvokNSJLaYaRDmMML+9gEAAAADAQAAAAAAAAAAAAAAAAAAAACWmoMzIYbul1Xkg5MlUlgG4Ymj0tK7S0dg6URD6X4cTyLe7vAFmo6XExAO2m4ZFE2n6KDvflObIHCLodjQIb0B");
    ASSERT_EQ(hex(output.hash()), "eea6e680f3ea51a7f667e9a801d0bfadf66e03d41ed54975b3c6006351461b32");
}

}
