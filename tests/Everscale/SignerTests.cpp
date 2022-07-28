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
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Everscale;

TEST(EverscaleSigner, Deploy) {
    auto key = PrivateKey(parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f"));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);

    auto bounce = false;
    auto flags = 3;
    auto amount = 1000000000;
    auto expiredAt = 1659026078;

    std::optional<Data> stateInit = {};
    std::optional<Address::MsgAddressInt> destination = {};

    auto data = createSignedMessage(publicKey, key, bounce, flags, amount, expiredAt, destination, stateInit);
    ASSERT_EQ(TW::Base64::encode(data), "iABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYAIyIRhBmub4CthfowsK/+nfwhiZ3IQvYenZ/V6xgpkf/hCkNtlh1+9IxezU0xS9A0e1mZD9f1QzBKNMJzMGmqAKXUlsUxcV1PAAAAAAHA==");
}
