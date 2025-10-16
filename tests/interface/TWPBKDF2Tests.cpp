// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/Generated/TWPBKDF2.h>

#include <gtest/gtest.h>

TEST(TWPBKDF2, Sha256_sha512) {
    auto password = DATA("50617373776f7264"); // Password
    auto salt = DATA("73616C74123456781123123412345678");

    auto sha256Result = WRAPD(TWPBKDF2HmacSha256(password.get(), salt.get(), 80000, 128));
    assertHexEqual(sha256Result, "76c865ae191831903180351695664a527f7b265fb266ad30afc3930496d475e2b5aad1de9c2eccb304f9bb6050840ca07c8c0819c19e5a9ce2537ac11f2a54aeea0e6054af492ecd5713cc20c2a5570ea038b9b908f4ef8c1768af9e74bb0d791fb3bd67a2e2e3d18dfda895d13d538513a9401951e18faf800e93e33ba66075");

    auto sha512Result = WRAPD(TWPBKDF2HmacSha512(password.get(), salt.get(), 80000, 128));
    assertHexEqual(sha512Result, "13f511d67457b60f3727e9b788b38e5f33cb6168f5617d8870435ad375fc516bb768ce1a5080232322c2ce7156033ac6cc6bee89ce45be42232ae77544810e25c64c24f414523d87c5e8240bb204a6d43d710ff2423bcd69c26dc0cbdae569ac1d52c8a5ce07b6126c7149b05f2024aad7053aca29b03fbcba21f72d973de1f5");
}
