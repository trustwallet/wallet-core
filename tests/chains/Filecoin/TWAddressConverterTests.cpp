// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFilecoinAddressConverter.h>

#include <gtest/gtest.h>
#include <vector>

#include "TestUtilities.h"

namespace TW::Filecoin::tests {
// clang-format off

struct AddressTest {
    const char* filecoin;
    const char* eth;
};

TEST(TWFilecoinAddressConverter, ConvertToEth) {
    const AddressTest tests[] = {
        {"f09876", "0xff00000000000000000000000000000000002694"},
        {"f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky", "0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0"},
        // The following addresses can't be converted to ETH. Expect an empty result.
        {"f15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq", ""},
        {"f24vg6ut43yw2h2jqydgbg2xq7x6f4kub3bg6as6i", ""},
        {"f3vvmn62lofvhjd2ugzca6sof2j2ubwok6cj4xxbfzz4yuxfkgobpihhd2thlanmsh3w2ptld2gqkn2jvlss4a", ""},
        // Should fail since `actorID != 10`. Expect an empty result.
        {"f432f77777777x32lpna", ""},
        {"f418446744073709551615ftnkyfaq", ""},
        // The following addresses are invalid. Expect an empty result.
        {"f432f77777777x32lpn", ""},
        {"f018446744073709551616", ""},
    };

    for (const auto& test : tests) {
        auto filecoinAddress = STRING(test.filecoin);
        auto result = WRAPS(TWFilecoinAddressConverterConvertToEthereum(filecoinAddress.get()));
        assertStringsEqual(result, test.eth);
    }
}

TEST(TWFilecoinAddressConverter, ConvertFromEth) {
    const AddressTest tests[] = {
        {"f410f74aaaaaaaaaaaaaaaaaaaaaaaaaaajuu3nnltyi", "0xff00000000000000000000000000000000002694"},
        {"f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky", "0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0"},
        {"f410f2oekwcmo2pueydmaq53eic2i62crtbeyuzx2gmy", "0xd388ab098ed3e84c0d808776440b48f685198498"},
        // The following addresses are invalid. Expect an empty result.
        {"", "0xd388ab098ed3e84c0d808776440b48f68519849"},
        {"", "0x"},
    };

    for (const auto& test : tests) {
        auto ethAddress = STRING(test.eth);
        auto result = WRAPS(TWFilecoinAddressConverterConvertFromEthereum(ethAddress.get()));
        assertStringsEqual(result, test.filecoin);
    }
}

}
