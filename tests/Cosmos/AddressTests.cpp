// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Cosmos/Address.h"

#include <gtest/gtest.h>
#include <TrustWalletCore/TWHDWallet.h>

namespace TW::Cosmos {

TEST(Address, Valid) {
    ASSERT_TRUE(Address::isValid("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2"));
}

TEST(Address, Invalid) {
    ASSERT_FALSE(Address::isValid("bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2"));
}

TEST(Address, FromKeyHash) {
    auto privateKey = PrivateKey(parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832"));
    auto publicKeyData = privateKey.getPublicKey(PublicKeyType::secp256k1);
    ASSERT_EQ(hex(publicKeyData.bytes.begin(), publicKeyData.bytes.end()), "026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502");

    auto publicKey = PublicKey(publicKeyData);
    auto address = Address("bnb", publicKey);
    ASSERT_EQ(address.string(), "bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2");
}

TEST(Address, FromKeyHashCosmos) {
    auto privateKey = PrivateKey(parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"));
    auto publicKeyData = privateKey.getPublicKey(PublicKeyType::secp256k1);
    ASSERT_EQ(hex(publicKeyData.bytes.begin(), publicKeyData.bytes.end()), "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5");

    auto publicKey = PublicKey(publicKeyData);
    auto address = Address("cosmos", publicKey);
    ASSERT_EQ(address.string(), "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
}

} // namespace TW::Cosmos
