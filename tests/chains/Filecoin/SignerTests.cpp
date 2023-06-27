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
    const Address address = Address::secp256k1Address(publicKey);
    ASSERT_EQ(address.string(), "f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq");
}

TEST(FilecoinSigner, Sign) {
    const PrivateKey privateKey(
        parse_hex("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe"));
    const PublicKey publicKey((privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)));
    const Address fromAddress = Address::secp256k1Address(publicKey);
    const Address toAddress("f1rletqqhinhagw6nxjcr4kbfws25thgt7owzuruy");

    Transaction tx(toAddress, fromAddress,
                   /*nonce*/ 1,
                   /*value*/ 6000,
                   /*gasLimit*/ 23423423423423,
                   /*gasFeeCap*/ 456456456456445645,
                   /*gasPremium*/ 5675674564734345,
                   /*method*/ Transaction::MethodType::SEND,
                   /*params*/ Data());

    Data signature = Signer::sign(privateKey, tx);

    ASSERT_EQ(
        tx.serialize(Transaction::SignatureType::SECP256K1, signature),
        R"({"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"456456456456445645","GasLimit":23423423423423,"GasPremium":"5675674564734345","Method":0,"Nonce":1,"To":"f1rletqqhinhagw6nxjcr4kbfws25thgt7owzuruy","Value":"6000"},"Signature":{"Data":"3GOUpn2Wiwe20QXLC8ixx23WiKDwrVkfxYi3CgzZ5jBVKZT4WUOZNuZhpUFky0PqGaM7vErEOi//yqBGSIQQUAA=","Type":1}})"
    );
}

/// Successfully broadcasted:
/// https://filfox.info/en/message/bafy2bzaceczvto7d2af7cq3kuwlvmanlh5xica4apl3vwxu37yaeozq72mvgm
TEST(FilecoinSigner, SignToDelegated) {
    Proto::SigningInput input;

    auto privateKey = parse_hex("d3d6ed8b97dcd4661f62a1162bee6949401fd3935f394e6eacf15b6d5005483c");
    auto toAddress = "f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky";
    uint64_t nonce = 0;
    // 0.001 FIL
    auto value = store(uint256_t(1'000'000) * uint256_t(1'000'000'000));
    uint64_t gasLimit = 6152567;
    auto gasFeeCap = store(uint256_t(4435940585));
    auto gasPremium = store(uint256_t(11597139));

    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_to(toAddress);
    input.set_nonce(nonce);
    input.set_value(value.data(), value.size());
    input.set_gas_limit(gasLimit);
    input.set_gas_fee_cap(gasFeeCap.data(), gasFeeCap.size());
    input.set_gas_premium(gasPremium.data(), gasPremium.size());

    Proto::SigningOutput output = Signer::sign(input);

    ASSERT_EQ(output.json(), R"({"Message":{"From":"f1mzyorxlcvdoqn5cto7urefbucugrcxxghpjc5hi","GasFeeCap":"4435940585","GasLimit":6152567,"GasPremium":"11597139","Method":3844450837,"Nonce":0,"To":"f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky","Value":"1000000000000000"},"Signature":{"Data":"bxZhnsOYjdArPa3W0SpggwqtXPgvfRSoM2dU5lXYar9lWhTGc6FvPWk2RTUGyA8UtzMIdOPSUKfzU1iA2eA3YwA=","Type":1}})");
}

/// Successfully broadcasted:
/// https://filfox.info/en/message/bafy2bzacea3ioez23o7t2hae6t2qwwkow46nhc42ffm5lqyxzzvrzblofnleu
TEST(FilecoinSigner, SignFromDelegated) {
    Proto::SigningInput input;

    auto privateKey = parse_hex("825d2bb32965764a98338139412c7591ed54c951dd65504cd8ddaeaa0fea7b2a");
    auto invalidToAddress = "f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq";
    auto toAddress = "f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky";
    uint64_t nonce = 0;
    // 0.001 FIL
    auto value = store(uint256_t(1'000'000) * uint256_t(1'000'000'000));
    uint64_t gasLimit = 2154192;
    auto gasFeeCap = store(uint256_t(516751679));
    auto gasPremium = store(uint256_t(12729639));

    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_nonce(nonce);
    input.set_value(value.data(), value.size());
    input.set_gas_limit(gasLimit);
    input.set_gas_fee_cap(gasFeeCap.data(), gasFeeCap.size());
    input.set_gas_premium(gasPremium.data(), gasPremium.size());
    input.set_derivation(Proto::DerivationType::DELEGATED);

    // Check if the output.error is set if `to` address is invalid.
    input.set_to(invalidToAddress);
    Proto::SigningOutput errorOutput = Signer::sign(input);
    ASSERT_FALSE(errorOutput.error_message().empty());
    ASSERT_TRUE(errorOutput.json().empty());

    // Set the valid `to` address. Expect to get a valid response.
    input.set_to(toAddress);
    Proto::SigningOutput output = Signer::sign(input);

    ASSERT_TRUE(output.error_message().empty());
    ASSERT_EQ(output.json(), R"({"Message":{"From":"f410fvak24cyg3saddajborn6idt7rrtfj2ptauk5pbq","GasFeeCap":"516751679","GasLimit":2154192,"GasPremium":"12729639","Method":3844450837,"Nonce":0,"To":"f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky","Value":"1000000000000000"},"Signature":{"Data":"fMOvlBAnc5OYDVEMaSSQURiqmrJbfktSPcUI2ptAoKtK0xl++cnTSKtqZyNV4yH0X5Ly2N2bqeNlFwAFANHoFAE=","Type":3}})");
}

} // namespace TW::Filecoin
