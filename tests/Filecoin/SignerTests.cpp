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
                   /*gasLimit*/ 23423423423423,
                   /*gasFeeCap*/ 456456456456445645,
                   /*gasPremium*/ 5675674564734345);

    Data signature = Signer::sign(privateKey, tx);

    ASSERT_EQ(hex(tx.serialize(signature)),
              "828a0055018ac93840e869c06b79b748a3c504b696bb339a7f5501cf01bf485f61435e6770b52615bf45"
              "5e043a236101430017701b0000154db0d523bf49000655a8ba8e851ecd48001429fef64aad8900405842"
              "01dc6394a67d968b07b6d105cb0bc8b1c76dd688a0f0ad591fc588b70a0cd9e630552994f859439936e6"
              "61a54164cb43ea19a33bbc4ac43a2fffcaa0464884105000");
}

} // namespace TW::Filecoin
