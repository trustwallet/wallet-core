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

    ASSERT_EQ(
        hex(tx.serialize(signature)),
        "828855018ac93840e869c06b79b748a3c504b696bb339a7f5501cf01bf485f61435e6770b52615bf455e043a23"
        "6101430017704200024200c80040584201477bcf9a71c9e19af77fbd92677230a9612d927877e439eb4aa30643"
        "0a4ffae42251194820df211d00075e88e68c9704a880448532ec0092045a1e00e8f1cbb900");
}

} // namespace TW::Filecoin
