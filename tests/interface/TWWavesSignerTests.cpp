// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "HexCoding.h"
#include "Waves/Address.h"
#include "Waves/Transaction.h"
#include "proto/Waves.pb.h"
#include "uint256.h"

#include <TrustWalletCore/TWWavesSigner.h>
#include <PrivateKey.h>
#include <gtest/gtest.h>

using namespace TW;

TEST(TWWavesSigner, Sign) {
    auto input = Waves::Proto::SigningInput();
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("83mqJpmgB5Mko1567sVAdqZxVKsT6jccXt3eFSi4G1zE"));
    
    const auto publicKeyCurve25519 = privateKey.getPublicKey(TWPublicKeyTypeCURVE25519);
    input.set_timestamp(int64_t(1559146613));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto &message = *input.mutable_transfer_message();
    message.set_amount(int64_t(100000000));
    message.set_asset("DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    message.set_fee(int64_t(100000));
    message.set_fee_asset("DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    message.set_to("3PPCZQkvdMJpmx7Zrz1cnYsPe9Bt1XT2Ckx");
    message.set_attachment("hello");

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWWavesSignerSign(inputTWData);
    auto output = Waves::Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    ASSERT_EQ(hex(output.signature()),
              "5d6a77b1fd9b53d9735cd2543ba94215664f2b07d6c7befb081221fcd49f5b6ad6b9ac108582e8d3e749"
              "43bdf35fd80d985edf4b4de1fb1c5c427e84d0879f8f");

    auto transaction = Waves::Transaction(
         input,
        /* pub_key */ publicKeyCurve25519.bytes);

    ASSERT_TRUE(publicKeyCurve25519.verify(
        Data(output.signature().begin(), output.signature().end()), transaction.serializeToSign()));
}
