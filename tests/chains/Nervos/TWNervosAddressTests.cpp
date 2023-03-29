// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWNervosAddress.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Nervos::tests {

TEST(TWNervosAddress, Create) {
    const auto ckbAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8furras980hksatlslfaktks7epf25";
    const auto addr = WRAP(TWNervosAddress, TWNervosAddressCreateWithString(STRING(ckbAddress).get()));

    const auto codeCash = WRAPD(TWNervosAddressCodeHash(addr.get()));
    const auto args = WRAPD(TWNervosAddressArgs(addr.get()));
    const auto hashType = WRAPS(TWNervosAddressHashType(addr.get()));

    EXPECT_TRUE(TWNervosAddressIsValidString(STRING(ckbAddress).get()));
    assertHexEqual(codeCash, "9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8");
    assertHexEqual(args, "c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da");
    assertStringsEqual(hashType, "type");
}

} // namespace TW::Nervos::tests
