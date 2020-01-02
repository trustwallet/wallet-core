// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWPKCS8.h>

#include <gtest/gtest.h>

auto pkcs8EncodedED25519PrivateKey = "302e020100300506032b657004220420d4ee72dbf913584ad5b6d8f1f769f8ad3afe7c28cbf1d4fbe097a88f44755842";
auto ed25519PrivateKey = "d4ee72dbf913584ad5b6d8f1f769f8ad3afe7c28cbf1d4fbe097a88f44755842";

TEST(TWPKCS8, DecodeED25519PrivateKey) {
    auto decoded = WRAPD(TWPKCS8DecodeED25519PrivateKey(DATA(pkcs8EncodedED25519PrivateKey).get()));
    assertHexEqual(decoded, ed25519PrivateKey);
}

TEST(TWPKCS8, EncodeED25519PrivateKey) {
    auto encoded = WRAPD(TWPKCS8EncodeED25519PrivateKey(DATA(ed25519PrivateKey).get()));
    assertHexEqual(encoded, pkcs8EncodedED25519PrivateKey);
}