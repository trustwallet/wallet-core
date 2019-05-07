// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWX509.h>

#include <gtest/gtest.h>

auto x509EncodedED25519PublicKey = "302a300506032b657003210019bf44096984cdfe8541bac167dc3b96c85086aa30b6b6cb0c5c38ad703166e1";
auto ed25519PublicKey = "19bf44096984cdfe8541bac167dc3b96c85086aa30b6b6cb0c5c38ad703166e1";

TEST(TWX509, DecodeED25519PublicKey) {
    auto decoded = WRAPD(TWX509DecodeED25519PublicKey(DATA(x509EncodedED25519PublicKey).get()));
    assertHexEqual(decoded, ed25519PublicKey);
}

TEST(TWX509, EncodeED25519PublicKey) {
    auto encoded = WRAPD(TWX509EncodeED25519PublicKey(DATA(ed25519PublicKey).get()));
    assertHexEqual(encoded, x509EncodedED25519PublicKey);
}