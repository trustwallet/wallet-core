// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Vite/Signer.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Vite;

const std::string kPrivateKey{"173c40e97fe2afcd24187e74f6b603cb949a5365e72fbdd065a6b165e2189e34"};

TEST(ViteSigner, sign1) {
    const auto privateKey = PrivateKey(parse_hex(kPrivateKey));

    auto input = Proto::SigningInput();
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto signer = Signer(input);
    ASSERT_EQ(hex(signer.blockHash), "f9a323153daefe041efb94d69b9669c882c935530ed953bbe8a665dfedda9696");
    const auto signature = signer.sign();
    ASSERT_EQ(hex(signature), "d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09");
    const Proto::SigningOutput out = signer.build();
    EXPECT_EQ(hex(out.signature()), "d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09");
}

TEST(ViteSigner, signInvalid1) {
    const auto privateKey = PrivateKey(parse_hex(kPrivateKey));

    auto input = Proto::SigningInput();
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    ASSERT_THROW(Signer signer(input), std::invalid_argument);
}
