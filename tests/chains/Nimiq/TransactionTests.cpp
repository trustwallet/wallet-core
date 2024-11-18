// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Nimiq/Address.h"
#include "Nimiq/Transaction.h"

#include <gtest/gtest.h>

namespace TW::Nimiq {

TEST(NimiqTransaction, PreImage) {
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
    ASSERT_EQ(hex(tx.getPreImage()),
        "000082d5f776378ccbe34a3d941f22d4715bc9f81e0d001450ffc385cd4e7c6ac9a7e91614ca67ff90568a0000000000028182ba00000000000003e80004cb2f180000");
}

TEST(NimiqTransaction, Serialize) {
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

    const auto signature = parse_hex("0ba678744be3bf9cd44fbcdabfb5be209f21739934836e26055610ab02720fa99489219d9f3581664473a1b40b30ad1f6e13150d59f8234a42c3f0de3d505405");
    std::copy(signature.begin(), signature.end(), tx.signature.begin());

    ASSERT_EQ(hex(tx.serialize()),
        "000070c7492aaa9c9ac7a05bc0d9c5db2dae9372029654f71f0c7f95deed5099b7021450ffc385cd4e7c6ac9a7e91614ca67ff90568a00000000028182ba00000000000003e80004cb2f180ba678744be3bf9cd44fbcdabfb5be209f21739934836e26055610ab02720fa99489219d9f3581664473a1b40b30ad1f6e13150d59f8234a42c3f0de3d505405");
}

} // namespace TW::Nimiq
