// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Filecoin/Address.h"
#include "Filecoin/Signer.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

namespace TW::Filecoin {

TEST(FilecoinSigner, DerivePublicKey) {
    const PrivateKey privateKey(
        parse_hex("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe"));
    const PublicKey publicKey((privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)));
    const Address address(publicKey);
    ASSERT_EQ(address.string(), "f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq");
}

TEST(FilecoinSigner, Sign) {
    const PrivateKey privateKey(
        parse_hex("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe"));
    const PublicKey publicKey((privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)));
    const Address fromAddress(publicKey);
    const Address toAddress("f1rletqqhinhagw6nxjcr4kbfws25thgt7owzuruy");

    Transaction tx(toAddress, fromAddress,
                   /*nonce*/ 1,
                   /*value*/ 6000,
                   /*gasPrice*/ 2,
                   /*gasLimit*/ 200);

    Data signature = Signer::sign(privateKey, tx);

    ASSERT_EQ(hex(tx.serialize(signature)),
              "82890055018ac93840e869c06b79b748a3c504b696bb339a7f5501cf01bf485f"
              "61435e6770b52615bf455e043a2361014300177042000218c80040584201c95e"
              "32222984251892b06b7da692e38003d6dd146da5f8a0bd67590099fa4a0937e0"
              "c075310a35a85c000b598e07920090cbc768231219a11e3ef7e9bed11da000");
}

} // namespace TW::Filecoin
