// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>
#include <regex>

#include "HexCoding.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWTezosMessageSigner.h>
#include <Tezos/MessageSigner.h>

namespace TW::Tezos::tests {
TEST(TezosMessageSigner, inputToPayload) {
    auto payload = Tezos::MessageSigner::inputToPayload("Tezos Signed Message: testUrl 2023-02-08T10:36:18.454Z Hello World");
    ASSERT_EQ(payload, "05010000004254657a6f73205369676e6564204d6573736167653a207465737455726c20323032332d30322d30385431303a33363a31382e3435345a2048656c6c6f20576f726c64");
}

TEST(TezosMessageSigner, formatMessage) {
    auto formatMessage = Tezos::MessageSigner::formatMessage("Hello World", "testUrl");
    std::regex regex("Tezos Signed Message: \\S+ \\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d{3}Z .+");
    ASSERT_TRUE(std::regex_match(formatMessage, regex));
}

TEST(TezosMessageSigner, SignMessage) {
    auto payload = Tezos::MessageSigner::inputToPayload("Tezos Signed Message: testUrl 2023-02-08T10:36:18.454Z Hello World");
    PrivateKey privKey(parse_hex("91b4fb8d7348db2e7de2693f58ce1cceb966fa960739adac1d9dba2cbaa0940a"));
    auto result = Tezos::MessageSigner::signMessage(privKey, payload);
    auto expected = "edsigu3se2fcEJUCm1aqxjzbHdf7Wsugr4mLaA9YM2UVZ9Yy5meGv87VqHN3mmDeRwApTj1JKDaYjqmLZifSFdWCqBoghqaowwJ";
    ASSERT_EQ(result, expected);
    ASSERT_TRUE(Tezos::MessageSigner::verifyMessage(privKey.getPublicKey(TWPublicKeyTypeED25519), payload, result));
}

TEST(TWTezosMessageSigner, formatMessage) {
    const auto message = STRING("Hello World");
    const auto dappUrl = STRING("testUrl");
    auto formattedMsg = WRAPS(TWTezosMessageSignerFormatMessage(message.get(), dappUrl.get()));
    std::regex regex("Tezos Signed Message: \\S+ \\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d{3}Z .+");
    ASSERT_TRUE(std::regex_match(std::string(TWStringUTF8Bytes(formattedMsg.get())), regex));
}

TEST(TWTezosMessageSigner, inputToPayload) {
    const auto message = STRING("Tezos Signed Message: testUrl 2023-02-08T10:36:18.454Z Hello World");
    const auto expected = "05010000004254657a6f73205369676e6564204d6573736167653a207465737455726c20323032332d30322d30385431303a33363a31382e3435345a2048656c6c6f20576f726c64";
    auto payload = WRAPS(TWTezosMessageSignerInputToPayload(message.get()));
    ASSERT_EQ(std::string(TWStringUTF8Bytes(payload.get())), expected);
}

TEST(TWTezosMessageSigner, SignAndVerify) {
    const auto privKeyData = "91b4fb8d7348db2e7de2693f58ce1cceb966fa960739adac1d9dba2cbaa0940a";
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
    const auto message = STRING("05010000004254657a6f73205369676e6564204d6573736167653a207465737455726c20323032332d30322d30385431303a33363a31382e3435345a2048656c6c6f20576f726c64");

    const auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeTezos));
    const auto signature = WRAPS(TWTezosMessageSignerSignMessage(privateKey.get(), message.get()));
    EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "edsigu3se2fcEJUCm1aqxjzbHdf7Wsugr4mLaA9YM2UVZ9Yy5meGv87VqHN3mmDeRwApTj1JKDaYjqmLZifSFdWCqBoghqaowwJ");
    EXPECT_TRUE(TWTezosMessageSignerVerifyMessage(pubKey.get(), message.get(), signature.get()));
}
} // namespace TW::Tezos::tests
