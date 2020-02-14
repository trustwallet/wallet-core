// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "HexCoding.h"
#include "proto/FreeCash.pb.h"
#include "FreeCash/Address.h"
#include "FreeCash/TransactionBuilder.h"
#include "FreeCash/TransactionSigner.h"

#include <TrustWalletCore/TWFreeCashAddress.h>
#include <TrustWalletCore/TWFreeCashCashAddress.h>
#include <TrustWalletCore/TWFreeCashScript.h>
#include <TrustWalletCore/TWFreeCashTransactionSigner.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

using namespace TW;


TEST(FreeCash, SignTransaction) {
    const int64_t amount = 56700;
    const int64_t fee = 226;

    auto input = FreeCash::Proto::SigningInput();
    input.set_hash_type(TWFreeCashSigHashTypeAll | TWFreeCashSigHashTypeFork);
    input.set_coin_type(TWCoinTypeFreeCash);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("FEMRWWpVHVVShfvVCPbqUkgezXJ2rJeTrJ");
    input.set_change_address("FEMRWWpVHVVShfvVCPbqUkgezXJ2rJeTrJ");

    // 5f84d17d261f5c141086edf393ef4aeecf11a253d46360e05c5e70b51222bece
    auto hash0 = DATA("cebe2212b5705e5ce06063d453a211cfee4aef93f3ed8610145c1f267dd1845f");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(1000000000);

    auto script0 = parse_hex("76a9145d7c8cc11e2f5f00af866f3d47e069e648c44ac388ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("c775e7b757ede630cd0aa1113bd102661ab38829ca52a6422ab782862f268646");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    // Sign
    auto signer = TW::FreeCash::TransactionSigner<TW::FreeCash::Transaction>(std::move(input));
    auto result = signer.sign();
    auto signedTx = result.payload();

    ASSERT_TRUE(result);
    ASSERT_EQ(fee, signer.plan.fee);

    Data serialized;
    signedTx.encode(false, serialized);
    ASSERT_EQ(hex(serialized), "");
}

