// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NEAR/Address.h"
#include "Base58.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWPublicKeyType.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::NEAR;

TEST(NEARAddress, Validation) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"));
    ASSERT_FALSE(Address::isValid("EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT"));

    ASSERT_TRUE(Address::isValid("NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v"));
}

TEST(NEARAddress, FromString) {
    ASSERT_EQ(
        Address("NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v").string(),
        "NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v"
    );
}

TEST(NEARAddress, FromPrivateKey) {
    auto fullKey = Base58::bitcoin.decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    auto key = PrivateKey(Data(fullKey.begin(), fullKey.begin() + 32));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v");
}
