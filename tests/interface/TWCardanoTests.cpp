// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCardanoAddress.h>
#include <TrustWalletCore/TWCardanoSigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "proto/Cardano.pb.h"
#include "TWTestUtilities.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Cardano;


TEST(TWCardano, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4").get()));
    ASSERT_NE(nullptr, privateKey.get());
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519Extended(privateKey.get());
    ASSERT_NE(nullptr, publicKey);
    ASSERT_EQ(64, publicKey->impl.bytes.size());
    auto address = WRAP(TWCardanoAddress, TWCardanoAddressCreateWithPublicKey(publicKey));
    auto addressString = WRAPS(TWCardanoAddressDescription(address.get()));
    assertStringsEqual(addressString, "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");

    auto address2 = WRAP(TWCardanoAddress, TWCardanoAddressCreateWithString(STRING("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W").get()));
    ASSERT_NE(nullptr, address2.get());
    auto address2String = WRAPS(TWCardanoAddressDescription(address2.get()));
    assertStringsEqual(address2String, "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");

    ASSERT_TRUE(TWCardanoAddressEqual(address.get(), address2.get()));
}

TEST(TWCardano, Sign) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_fee((uint64_t)(0.169884 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    auto utxo = input.add_utxo();
    utxo->mutable_out_point()->set_txid("59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14");
    utxo->mutable_out_point()->set_index(6);
    utxo->set_amount((uint64_t)(15.0 * 1000000));
    Data privateKey = parse_hex("b8c31abcc41d931ae881be11da9e4d9242b1f01cae4e69fa29d5ba1f89f9c1549ec844c6b39c70fa6d3a254fe57c1efee1a75eb9755e0b751e96dd288deabc881ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f");
    input.add_private_key(privateKey.data(), privateKey.size());
    
    auto inputString = input.SerializeAsString();
    auto inputData = TWDataCreateWithBytes((const byte *)inputString.data(), inputString.size());
    auto outputData = TWCardanoSignerSign(inputData);
    auto output = Proto::SigningOutput();
    ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData), TWDataSize(outputData)));

    EXPECT_EQ(
        "82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d307e4ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f5840a60e06de3986d77c6f126d98c3107749ec13f8593a1d33e00e3c0c2ac5d2d41c3bcbd52afe2439b8afb816da2e6701fdd01092fa8d305f08b8fd41f249282709",
        hex(output.encoded())
    );
    EXPECT_EQ("d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f", output.transaction_id());
}
