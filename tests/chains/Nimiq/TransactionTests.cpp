// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        "000082d5f776378ccbe34a3d941f22d4715bc9f81e0d001450ffc385cd4e7c6ac9a7e91614ca67ff90568a0000000000028182ba00000000000003e80004cb2f2a00");
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

    const auto signature = parse_hex("74dc7f6e0ab58a0bf52cc6e8801b0cca132dd4229d9a3e3a3d2f90e4d8f045d981b771bf5fc3851a98f3c617b1a943228f963e910e061808a721cfa0e3cad50b");
    std::copy(signature.begin(), signature.end(), tx.signature.begin());

    ASSERT_EQ(hex(tx.serialize()),
        "0070c7492aaa9c9ac7a05bc0d9c5db2dae9372029654f71f0c7f95deed5099b7021450ffc385cd4e7c6ac9a7e91614ca67ff90568a00000000028182ba00000000000003e80004cb2f2a74dc7f6e0ab58a0bf52cc6e8801b0cca132dd4229d9a3e3a3d2f90e4d8f045d981b771bf5fc3851a98f3c617b1a943228f963e910e061808a721cfa0e3cad50b");
}

} // namespace TW::Nimiq
