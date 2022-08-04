// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Everscale.pb.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Everscale {

TEST(TWAnySignerEverscale, SignMessage) {
    Proto::SigningInput input;

    auto& transfer = *input.mutable_transfer();
    transfer.set_bounce(false);
    transfer.set_flags(3);
    transfer.set_amount(1000000000);
    transfer.set_expired_at(1659026078);

    auto privateKey = parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f");
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeEverscale);

    ASSERT_EQ(output.encoded(), "iABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYAIyIRhBmub4CthfowsK/+nfwhiZ3IQvYenZ/V6xgpkf/hCkNtlh1+9IxezU0xS9A0e1mZD9f1QzBKNMJzMGmqAKXUlsUxcV1PAAAAAAHA==");
}

}