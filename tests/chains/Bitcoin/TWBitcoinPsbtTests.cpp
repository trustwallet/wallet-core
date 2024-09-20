// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "proto/BitcoinV2.pb.h"
#include "PrivateKey.h"
#include "TestUtilities.h"

#include "TrustWalletCore/TWBitcoinPsbt.h"

#include <gtest/gtest.h>

namespace TW::Bitcoin::PsbtTests {

const auto gPrivateKey = PrivateKey(parse_hex("f00ffbe44c5c2838c13d2778854ac66b75e04eb6054f0241989e223223ad5e55"));
const auto gPsbt = parse_hex("70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d00000000");

TEST(TWBitcoinPsbt, SignThorSwap) {
    BitcoinV2::Proto::PsbtSigningInput input;
    input.set_psbt(gPsbt.data(), gPsbt.size());
    input.add_private_keys(gPrivateKey.bytes.data(), gPrivateKey.bytes.size());

    const auto inputData = data(input.SerializeAsString());
    const auto inputPtr = WRAPD(TWDataCreateWithBytes(inputData.data(), inputData.size()));

    const auto outputPtr = WRAPD(TWBitcoinPsbtSign(inputPtr.get(), TWCoinTypeBitcoin));

    BitcoinV2::Proto::PsbtSigningOutput output;
    output.ParseFromArray(
        TWDataBytes(outputPtr.get()),
        static_cast<int>(TWDataSize(outputPtr.get()))
    );

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(hex(output.psbt()), "70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d01086c02483045022100b1229a008f20691639767bf925d6b8956ea957ccc633ad6b5de3618733a55e6b02205774d3320489b8a57a6f8de07f561de3e660ff8e587f6ac5422c49020cd4dc9101210306d8c664ea8fd2683eebea1d3114d90e0a5429e5783ba49b80ddabce04ff28f300000000");
    EXPECT_EQ(hex(output.encoded()), "02000000000101147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d02483045022100b1229a008f20691639767bf925d6b8956ea957ccc633ad6b5de3618733a55e6b02205774d3320489b8a57a6f8de07f561de3e660ff8e587f6ac5422c49020cd4dc9101210306d8c664ea8fd2683eebea1d3114d90e0a5429e5783ba49b80ddabce04ff28f300000000");
}

TEST(TWBitcoinPsbt, PlanThorSwap) {
    const auto publicKey = gPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    BitcoinV2::Proto::PsbtSigningInput input;
    input.set_psbt(gPsbt.data(), gPsbt.size());
    input.add_public_keys(publicKey.bytes.data(), publicKey.bytes.size());

    const auto inputData = data(input.SerializeAsString());
    const auto inputPtr = WRAPD(TWDataCreateWithBytes(inputData.data(), inputData.size()));

    const auto planPtr = WRAPD(TWBitcoinPsbtPlan(inputPtr.get(), TWCoinTypeBitcoin));

    BitcoinV2::Proto::TransactionPlan plan;
    plan.ParseFromArray(
        TWDataBytes(planPtr.get()),
        static_cast<int>(TWDataSize(planPtr.get()))
    );

    EXPECT_EQ(plan.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(plan.send_amount(), 66'406);
    EXPECT_EQ(plan.fee_estimate(), 1'736);
}

}
