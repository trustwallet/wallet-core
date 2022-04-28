// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BigInt.h"

namespace TW::Arweave {


BigInt bigIntFromData(const Data& data) {
    BigInt num;
    import_bits(num, data.begin(), data.end(), 0, true);
    return num;
}

// First byte is sign, ignored
BigInt bigIntFromDataWithSign(const Data& data) {
    if (data.size() < 2) {
        return BigInt(0);
    }
    Data dcopy = data;
    dcopy[0] = 0;
    return bigIntFromData(dcopy);
}

bool bigIntTestBit(const BigInt& num, int bit) {
    return bool((num >> bit) & 1);
}

} // namespace
