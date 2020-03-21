// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include "../interface/TWTestUtilities.h"
#include "BitcoinGold/SegwitAddress.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"
#include <gtest/gtest.h>
#include <TrustWalletCore/TWAnyAddress.h>

using namespace TW;


// {
//   "address": "btg1qtesn92ddy8m5yvypgsdtft3zj5qldj9g2u52sk",
//   "scriptPubKey": "00145e6132a9ad21f7423081441ab4ae229501f6c8a8",
//   "ismine": true,
//   "iswatchonly": false,
//   "isscript": false,
//   "iswitness": true,
//   "witness_version": 0,
//   "witness_program": "5e6132a9ad21f7423081441ab4ae229501f6c8a8",
//   "pubkey": "02f74712b5d765a73b52a14c1e113f2ef3f9502d09d5987ee40f53828cfe68b9a6",
//   "label": "",
//   "timestamp": 1520169519,
//   "hdkeypath": "m/0'/0'/7'",
//   "hdseedid": "78087f46f7ca96b35bd3a5ab2331f7c96780166d",
//   "hdmasterkeyid": "78087f46f7ca96b35bd3a5ab2331f7c96780166d",
//   "labels": [
//     {
//       "name": "",
//       "purpose": "receive"
//     }
//   ]
// }

TEST(TWBitcoinGoldSegwitAddress, Valid) {
    ASSERT_TRUE(BitcoinGold::SegwitAddress::isValid("btg1qtesn92ddy8m5yvypgsdtft3zj5qldj9g2u52sk"));
    ASSERT_FALSE(BitcoinGold::SegwitAddress::isValid("btg1qtesn92ddy8m5yvypgsdtft3zj5qldj9g2u52sl"));
}

/// Initializes a Bech32 address with a human-readable part, a witness
/// version, and a witness program.
TEST(TWBitcoinGoldSegwitAddress, WitnessProgramToAddress) {
    auto address = BitcoinGold::SegwitAddress("btg", 0, parse_hex("5e6132a9ad21f7423081441ab4ae229501f6c8a8"));

    ASSERT_TRUE(BitcoinGold::SegwitAddress::isValid(address.string()));
    ASSERT_EQ(address.string(), "btg1qtesn92ddy8m5yvypgsdtft3zj5qldj9g2u52sk");
}

/// Initializes a Bech32 address with a public key and a HRP prefix.
TEST(TWBitcoinGoldSegwitAddress, PubkeyToAddress) {
    const auto publicKey = PublicKey(parse_hex("02f74712b5d765a73b52a14c1e113f2ef3f9502d09d5987ee40f53828cfe68b9a6"), TWPublicKeyTypeSECP256k1);

    /// construct with public key
    auto address = BitcoinGold::SegwitAddress(publicKey, 0, "btg");

    ASSERT_TRUE(BitcoinGold::SegwitAddress::isValid(address.string()));
    ASSERT_EQ(address.string(), "btg1qtesn92ddy8m5yvypgsdtft3zj5qldj9g2u52sk");
}

/// Decodes a SegWit address.
TEST(TWBitcoinGoldSegwitAddress, Decode) {
    std::pair<Bitcoin::SegwitAddress, bool> result = BitcoinGold::SegwitAddress::decode("btg1qtesn92ddy8m5yvypgsdtft3zj5qldj9g2u52sk");

    ASSERT_TRUE(result.second);
    ASSERT_EQ(result.first.string(), "btg1qtesn92ddy8m5yvypgsdtft3zj5qldj9g2u52sk");
}
