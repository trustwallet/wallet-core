// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PublicKey.h"
#include "PublicKeyLegacy.h"
#include "NEO/Address.h"
#include "NEO/Signer.h"

#include <gtest/gtest.h>

namespace TW::NEO::tests {

using namespace std;

TEST(NEOSigner, FromPublicPrivateKey) {
    auto hexPrvKey = "4646464646464646464646464646464646464646464646464646464646464646";
    auto hexPubKey = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
    auto signer = Signer(PrivateKey(parse_hex(hexPrvKey)));
    auto prvKey = signer.getPrivateKey();
    auto pubKey = signer.getPublicKey();

    EXPECT_EQ(hexPrvKey, hex(prvKey.bytes));
    EXPECT_EQ(hexPubKey, hex(pubKey.bytes));

    auto address = signer.getAddress();
    EXPECT_TRUE(Address::isValid(address.string()));

    EXPECT_EQ(Address(pubKey), address);
}

TEST(NEOSigner, SigningData) {
    auto signer = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646")));
    auto verScript = "ba7908ddfe5a1177f2c9d3fa1d3dc71c9c289a3325b3bdd977e20c50136959ed02d1411efa5e8b897d970ef7e2325e6c0a3fdee4eb421223f0d86e455879a9ad";
    auto invocationScript = string("401642b3d538e138f34b32330e381a7fe3f5151fcf958f2030991e72e2e25043143e4a1ebd239634efba279c96fa0ab04a15aa15179d73a7ef5a886ac8a06af484401642b3d538e138f34b32330e381a7fe3f5151fcf958f2030991e72e2e25043143e4a1ebd239634efba279c96fa0ab04a15aa15179d73a7ef5a886ac8a06af484401642b3d538e138f34b32330e381a7fe3f5151fcf958f2030991e72e2e25043143e4a1ebd239634efba279c96fa0ab04a15aa15179d73a7ef5a886ac8a06af484");
    invocationScript = string(invocationScript.rbegin(), invocationScript.rend());

    EXPECT_EQ(verScript, hex(signer.sign(parse_hex(invocationScript))));
}

TEST(NEOAccount, validity) {
    auto hexPrvKey = "4646464646464646464646464646464646464646464646464646464646464646";
    auto hexPubKey = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
    auto signer = Signer(PrivateKey(parse_hex(hexPrvKey)));
    auto prvKey = signer.getPrivateKey();
    auto pubKey = signer.getPublicKey();
    EXPECT_EQ(hexPrvKey, hex(prvKey.bytes));
    EXPECT_EQ(hexPubKey, hex(pubKey.bytes));
}

TEST(NEOSigner, SigningTransaction) {
    auto privateKey = PrivateKey(parse_hex("F18B2F726000E86B4950EBEA7BFF151F69635951BC4A31C44F28EE6AF7AEC128"));
    auto signer = Signer(privateKey);
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x00;

    CoinReference coin;
    coin.prevHash = load(parse_hex("9c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de")); // reverse hash
    coin.prevIndex = (uint16_t)1;
    transaction.inInputs.push_back(coin);

    {
        TransactionOutput out;
        out.assetId = load(parse_hex("9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5"));
        out.value = (int64_t)1 * 100000000;
        auto scriptHash = TW::NEO::Address("Ad9A1xPbuA5YBFr1XPznDwBwQzdckAjCev").toScriptHash();
        out.scriptHash = load(scriptHash);
        transaction.outputs.push_back(out);
    }

    {
        TransactionOutput out;
        out.assetId = load(parse_hex("9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5"));
        out.value = (int64_t)892 * 100000000;
        auto scriptHash = TW::NEO::Address("AdtSLMBqACP4jv8tRWwyweXGpyGG46eMXV").toScriptHash();
        out.scriptHash = load(scriptHash);
        transaction.outputs.push_back(out);
    }

    signer.sign(transaction);
    auto signedTx = transaction.serialize();
    EXPECT_EQ(hex(signedTx), "800000019c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de0100029b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000ea610aa6db39bd8c8556c9569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500fcbbc414000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac0141405046619c8e20e1fdeec92ce95f3019f6e7cc057294eb16b2d5e55c105bf32eb27e1fc01c1858576228f1fef8c0945a8ad69688e52a4ed19f5b85f5eff7e961d7232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac");
    // TODO uncomment when nist256p1 Rust implementation is enabled.
    // EXPECT_EQ(hex(signedTx), "800000019c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de0100029b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000ea610aa6db39bd8c8556c9569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500fcbbc414000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac0141405046619c8e20e1fdeec92ce95f3019f6e7cc057294eb16b2d5e55c105bf32eb281e03fe2e7a7a89ed70e01073f6ba574e65071c87cc8cce59833d4d30479c37a232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac");
}

} // namespace TW::NEO::tests
