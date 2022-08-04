// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Everscale/Signer.h"
#include "Everscale/Address.h"
#include "Everscale/Messages.h"

#include "Base64.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Everscale {

TEST(EverscaleSigner, WithoutStateInit) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(1000000000);
    transfer.set_expired_at(1659026078);

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(output.encoded(), "iABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYAIyIRhBmub4CthfowsK/+nfwhiZ3IQvYenZ/V6xgpkf/hCkNtlh1+9IxezU0xS9A0e1mZD9f1QzBKNMJzMGmqAKXUlsUxcV1PAAAAAAHA==");
}

TEST(EverscaleSigner, WithStateInit1) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(1000000000);
    transfer.set_expired_at(1659026078);

    auto dst = Address("0:6dc497fb5cdb013fb1b807d8a2dbf7653d3e112cdb6ca95054bd2c74408d557d");
    transfer.set_address(dst.string().c_str(), dst.string().size());

    auto stateInit = TW::Base64::decode("te6ccgEBAQEAKgAAUAAAAi9LqS2K8drjTwv63I+aPTBLtMULU+zuEMSAmO8j5A00qizUXzU=");
    transfer.set_state_init(stateInit.data(), stateInit.size());

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(output.encoded(), "iABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYBGC+o+gbLqcs2fFi5y5dyYccszRvGc4IBmWqCU/mWGub29BSl+RG2pv6zkYPUVwox/ExjTZfBXR9U+vUBKQ1VgBSXUlsUxcV1PAAABF4HA=");
}

TEST(EverscaleSigner, WithStateInit2) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(1000000000);
    transfer.set_expired_at(1659026078);

    auto dst = Address("0:ab91c8cec44aa6e3b2c31443202a63241394491f0e41215a30620b57ef28b69b");
    transfer.set_address(dst.string().c_str(), dst.string().size());

    auto stateInit = TW::Base64::decode("te6ccgEBAQEAKgAAUAAAATZLqS2Kc7K5TorVBJDDavbyxCCgDW/rqoGMkiZpeu4aVpmmWvE=");
    transfer.set_state_init(stateInit.data(), stateInit.size());

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);
    auto boc = output.encoded();

    ASSERT_EQ(output.encoded(), "iABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYBGFRS7u+U3g33tkvU5mOIulqQY3bE292gaWV7+cPYFxLXr/KwApb9WXBy73l6ZjBMneTnpTaDu9oeTDxl5QLVfgyXUlsUxcV1PAAAAmwHA=");
}

TEST(EverscaleSigner, WithStateInit3) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(1000000000);
    transfer.set_expired_at(1659026078);

    auto dst = Address("0:ab91c8cec44aa6e3b2c31443202a63241394491f0e41215a30620b57ef28b69b");
    transfer.set_address(dst.string().c_str(), dst.string().size());

    auto stateInit = TW::Base64::decode("te6ccgEBAQEAKgAAUAAAAAFLqS2KXxBKs/v1orUAXustiAzDcUQaU1+G6pNGNe9MHTMhL6c=");
    transfer.set_state_init(stateInit.data(), stateInit.size());

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(output.encoded(), "iABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYBGfvqM+5hsqYZqAqU6ISgqIPWhSGBCFWjZA70I++alUCYqbAMl6Cd+/j9V9Ay28HwvehRNPQmdWydq2aZHQt1dByXUlsUxcV1PAAAAAIHA=");
}

TEST(EverscaleSigner, WithStateInit4) {
    auto input = Proto::SigningInput();

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(1000000000);
    transfer.set_expired_at(1659026078);

    auto dst = Address("0:ab91c8cec44aa6e3b2c31443202a63241394491f0e41215a30620b57ef28b69b");
    transfer.set_address(dst.string().c_str(), dst.string().size());

    auto stateInit = TW::Base64::decode("te6ccgEBAQEAKgAAUAAAAAlLqS2K/tcFRfvWsRw2zYTUoXF4sEQQWx/uu+cyy+zwsarnBQk=");
    transfer.set_state_init(stateInit.data(), stateInit.size());

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(output.encoded(), "iABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYBGA/zSxfc0nSJb7kigVwlIV+5/87mtezg4DFLGE9i8o+H0DmW4pcqwXYvqiiUsED9QXUcfnkOdpFrVP49UJ8MpAiXUlsUxcV1PAAAABIHA=");
}

}
