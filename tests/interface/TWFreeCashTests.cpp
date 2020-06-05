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

    // 5f84d17d261f5c141086edf393ef4aeecf11a253d46360e05c5e70b51222bece/cebe2212b5705e5ce06063d453a211cfee4aef93f3ed8610145c1f267dd1845f
    // a9967f9d48cab4eca02d54ec158a1178ef3b4cfbbd9aa72d6b896197df0707b3/b30707df9761896b2da79abdfb4c3bef78118a15ec542da0ecb4ca489d7f96a9
    // 83c707056fa6a682c2d0fca4f0e1d6308a1cc4ef6523d430a96dd7249de0e489/89e4e09d24d76da930d42365efc41c8a30d6e1f0a4fcd0c282a6a66f0507c783
    auto hash0 = DATA("89e4e09d24d76da930d42365efc41c8a30d6e1f0a4fcd0c282a6a66f0507c783");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(899942848);

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
    ASSERT_EQ(hex(serialized), "020000000189e4e09d24d76da930d42365efc41c8a30d6e1f0a4fcd0c282a6a66f0507c78301000000644147eda8fa998d10e67d904bca1601ee354c1da2eb21fdbfacc571a02a4a970c2e6469aa656076650f832d4fd28cb6b5a0e44f361d752b7b386390cc97a68106bb412103de641a2dbbf3846635a509d423482fdc432df69eacfd89f0a7c79e952581e823ffffffff027cdd0000000000001976a9145d7c8cc11e2f5f00af866f3d47e069e648c44ac388ac622ba335000000001976a9145d7c8cc11e2f5f00af866f3d47e069e648c44ac388ac00000000");
}

