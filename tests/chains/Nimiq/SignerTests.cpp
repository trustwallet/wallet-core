// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Nimiq/Address.h"
#include "Nimiq/Signer.h"
#include "Nimiq/Transaction.h"

#include <gtest/gtest.h>

namespace TW::Nimiq {

TEST(NimiqSigner, DerivePublicKey) {
    const PrivateKey privateKey(parse_hex("e3cc33575834add098f8487123cd4bca543ee859b3e8cfe624e7e6a97202b756"));
    const PublicKey publicKey((privateKey.getPublicKey(TWPublicKeyTypeED25519)));
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "NQ27 GBAY EVHP HK5X 6JHV JGFJ 5M3H BF4Y G7GD");
}

TEST(NimiqSigner, Sign) {
    const PrivateKey privateKey(parse_hex("e3cc33575834add098f8487123cd4bca543ee859b3e8cfe624e7e6a97202b756"));
    const auto pubkey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    std::array<uint8_t, 32> pubkeyBytes;
    std::copy(pubkey.bytes.begin(), pubkey.bytes.end(), pubkeyBytes.data());

    Transaction tx(
        pubkeyBytes,
        Address("NQ86 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0MLA"),
        42042042,
        1000,
        314159
    );

    Signer signer;
    signer.sign(privateKey, tx);

    ASSERT_EQ(hex(tx.signature),
        "0ba678744be3bf9cd44fbcdabfb5be209f21739934836e26055610ab02720fa99489219d9f3581664473a1b40b30ad1f6e13150d59f8234a42c3f0de3d505405");
}

} // namespace TW::Nimiq
