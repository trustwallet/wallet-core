// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWString.h>

static inline uint8_t value(uint8_t c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;

    // Invalid digit
    return 0;
 }

TWData *TWDataCreateWithHexString(const TWString *hex) {
    size_t stringIndex = 0;
    if (TWStringSize(hex) >= 2 && TWStringGet(hex, 0) == '0' && TWStringGet(hex, 1) == 'x') {
        stringIndex += 2;
    }

    const size_t count = (TWStringSize(hex) - stringIndex + 1) / 2;
    TWData *data = TWDataCreateWithSize(count);

    size_t dataIndex = 0;
    while (stringIndex < TWStringSize(hex)) {
        uint8_t high = value(TWStringGet(hex, stringIndex));
        stringIndex += 1;
        if (stringIndex >= TWStringSize(hex)) {
            TWDataSet(data, dataIndex, high);
            break;
        }

        uint8_t low = value(TWStringGet(hex, stringIndex));
        stringIndex += 1;

        TWDataSet(data, dataIndex, (high << 4) | low);
        dataIndex += 1;
    }

    return data;
}
