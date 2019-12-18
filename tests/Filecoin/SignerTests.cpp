// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Filecoin/Address.h"
#include "Filecoin/Signer.h"

#include <gtest/gtest.h>

namespace TW::Filecoin {

TEST(FilecoinSigner, DerivePublicKey) {
    const PrivateKey privateKey(parse_hex("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe"));
    const PublicKey publicKey((privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)));
    const Address address(publicKey);
    ASSERT_EQ(address.string(), "f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq");
}

TEST(FilecoinSigner, Sign) {
    const PrivateKey privateKey(parse_hex("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe"));
    const PublicKey publicKey((privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)));
    const Address fromAddress(publicKey);
    const Address toAddress("f1rletqqhinhagw6nxjcr4kbfws25thgt7owzuruy");

    Transaction tx(
        toAddress, fromAddress,
        0x1334565890, /*nonce*/
        6000, /*value*/
        2, /*gas_price*/
        80 /*gas_limit*/
    );

    Signer signer;
    signer.sign(privateKey, tx);

    ASSERT_EQ(hex(tx.serialize()),
        "828855018ac93840e869c06b79b748a3c504b696bb339a7f5501cf01bf485f61435e6770b52615bf455e043a23611b0000001334565890430017704200024200500040584201c1764de3891d32fcb0b372d1515cb480ef485ed9d8d3a78befd1530382163f3b00c7ad178a74fa8a2eb229bb06cae71cacf2fba1493ff22bb6c290d519b24a2c01");
}

} // namespace TW::Filecoin
