// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Semux/Address.cpp"
#include "Semux/Transaction.cpp"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWPKCS8.h>

#include <gtest/gtest.h>

TEST(SemuxTransaction, GetPreImage) {
    auto preimageHex = "000114db7cadb25fdcdd546fb0268524107582c3f8999c000000000000000200000000004c4"
                       "b400000000000000001000001629b9257d000";
    auto to = Address("0xdb7cadb25fdcdd546fb0268524107582c3f8999c");
    uint64_t value = 2L;
    uint64_t fee = 5000000L;
    uint64_t nonce = 1L;
    uint64_t timestamp = 1523028482000L;

    const auto tx = Transaction(to, value, fee, nonce, timestamp);
    auto preimage = tx.getPreImage();
    ASSERT_EQ(hex(preimage), preimageHex);
}

TEST(SemuxTransaction, Serialize) {
    auto privateKeyHex = "302e020100300506032b6570042204200c186aa27ac15f862ec511252f0a8808fb7abb404"
                         "941a5ef0cfe1b1763cc1f68";
    auto serializedHex =
        "2039f501f17f544371367fb30493f03b2fc7c3fb76bed58e90fdf865c53858b48138000114db7cadb25fdcdd54"
        "6fb0268524107582c3f8999c000000000000000200000000004c4b400000000000000001000001629b9257d000"
        "606682b666cd5322fc65d442976f5aeb67dd526fec2d79322b1ce5d0442093051dae77719600f83390f6c5a84d"
        "c910ef448c81afc6bd966e56fb7de529a01a3503aa45b2f1e3682e6f3a1c31348fc42ec4ac2db57e48076135e5"
        "dc7649e81ab5d3";
    auto to = Address("0xdb7cadb25fdcdd546fb0268524107582c3f8999c");
    uint64_t value = 2L;
    uint64_t fee = 5000000L;
    uint64_t nonce = 1L;
    uint64_t timestamp = 1523028482000L;

    auto tx = Transaction(to, value, fee, nonce, timestamp);
    const auto signer = Signer();
    const auto pkcs8PrivateKeyBytes = parse_hex(privateKeyHex);
    const auto privateKeyBytes = *reinterpret_cast<const Data *>(TWPKCS8DecodeED25519PrivateKey(
        TWDataCreateWithBytes(pkcs8PrivateKeyBytes.data(), pkcs8PrivateKeyBytes.size())));
    auto privateKey = PrivateKey(privateKeyBytes);
    signer.sign(privateKey, tx);

    ASSERT_EQ(hex(tx.serialize()), serializedHex);
}
