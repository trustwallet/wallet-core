// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWString.h>
#include <gtest/gtest.h>

#include <vector>

#define WRAP(type, x) std::shared_ptr<type>(x, type##Delete)
#define WRAPD(x) std::shared_ptr<TWData>(x, TWDataDelete)
#define WRAPS(x) std::shared_ptr<TWString>(x, TWStringDelete)
#define STRING(x) std::shared_ptr<TWString>(TWStringCreateWithUTF8Bytes(x), TWStringDelete)
#define DATA(x) std::shared_ptr<TWData>(TWDataCreateWithHexString(STRING(x).get()), TWDataDelete)

inline void assertStringsEqual(std::shared_ptr<TWString>& string, const char* expected) {
    ASSERT_STREQ(TWStringUTF8Bytes(string.get()), expected);
}

inline void assertHexEqual(std::shared_ptr<TWData>& data, const char* expected) {
    auto hex = WRAPS(TWStringCreateWithHexData(data.get()));
    assertStringsEqual(hex, expected);
}

inline std::vector<uint8_t>* dataFromTWData(TWData* data) {
    return const_cast<std::vector<uint8_t>*>(reinterpret_cast<const std::vector<uint8_t>*>(data));
}
