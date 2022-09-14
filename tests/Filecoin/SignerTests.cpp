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

    ASSERT_EQ(tx.serialize(signature), R"({"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"456456456456445645","GasLimit":23423423423423,"GasPremium":"5675674564734345","Nonce":1,"To":"f1rletqqhinhagw6nxjcr4kbfws25thgt7owzuruy","Value":"6000"},"Signature":{"Data":"3GOUpn2Wiwe20QXLC8ixx23WiKDwrVkfxYi3CgzZ5jBVKZT4WUOZNuZhpUFky0PqGaM7vErEOi//yqBGSIQQUAA=","Type":1}})");
}

} // namespace TW::Filecoin
