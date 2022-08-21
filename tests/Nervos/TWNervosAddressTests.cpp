// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWNervosAddress.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Nervos::tests {

const auto ckbAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8furras980hksatlslfaktks7epf25";

TEST(TWBitcoinAddress, Create) {
    TWNervosAddress* addr = TWNervosAddressCreateWithString(STRING(ckbAddress).get());

    const auto codeCash = WRAPD(TWNervosAddressCodeHash(addr));
    const auto args = WRAPD(TWNervosAddressArgs(addr));
    const auto hashType = WRAPS(TWNervosAddressHashType(addr));

    assertHexEqual(codeCash, "9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8");
    assertHexEqual(args, "c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da");
    assertStringsEqual(hashType, "type");

    TWNervosAddressDelete(addr);
}

} // namespace TW::Nervos::tests
