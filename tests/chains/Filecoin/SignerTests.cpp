// Copyright © 2017-2023 Trust.
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
                   /*gasPremium*/ 5675674564734345,
                   /*method*/ Transaction::SEND_METHOD);

    Data signature = Signer::sign(privateKey, tx);

    ASSERT_EQ(tx.serialize(signature), R"({"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"456456456456445645","GasLimit":23423423423423,"GasPremium":"5675674564734345","Method":0,"Nonce":1,"To":"f1rletqqhinhagw6nxjcr4kbfws25thgt7owzuruy","Value":"6000"},"Signature":{"Data":"3GOUpn2Wiwe20QXLC8ixx23WiKDwrVkfxYi3CgzZ5jBVKZT4WUOZNuZhpUFky0PqGaM7vErEOi//yqBGSIQQUAA=","Type":1}})");
}

TEST(FilecoinSigner, SignToDelegated) {
    const PrivateKey privateKey(
        parse_hex("d3d6ed8b97dcd4661f62a1162bee6949401fd3935f394e6eacf15b6d5005483c"));
    const PublicKey publicKey((privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)));
    const Address fromAddress(publicKey);
    const Address toAddress("f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky");

    Transaction tx(toAddress, fromAddress,
        /*nonce*/ 0,
        /*value*/ 1000000000000000,
        /*gasLimit*/ 6152567,
        /*gasFeeCap*/ 4435940585,
        /*gasPremium*/ 11597139,
        /*method*/ Transaction::INVOKE_EVM_METHOD);

    Data signature = Signer::sign(privateKey, tx);

    ASSERT_EQ(tx.serialize(signature), R"({"Message":{"From":"f1mzyorxlcvdoqn5cto7urefbucugrcxxghpjc5hi","GasFeeCap":"4435940585","GasLimit":6152567,"GasPremium":"11597139","Method":3844450837,"Nonce":0,"To":"f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky","Value":"1000000000000000"},"Signature":{"Data":"bxZhnsOYjdArPa3W0SpggwqtXPgvfRSoM2dU5lXYar9lWhTGc6FvPWk2RTUGyA8UtzMIdOPSUKfzU1iA2eA3YwA=","Type":1}})");
}

} // namespace TW::Filecoin
