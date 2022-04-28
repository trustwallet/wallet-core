// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Prime.h"

#include <cassert>

namespace TW::Arweave {

BigInt primeCandidateFromInt(const BigInt& num, int bits) {
    BigInt p = num;
    const auto firstBit = bits - 1;
    if (!bigIntTestBit(p, firstBit)) {
        // set first bit to 1
        p += BigInt(1) << firstBit;
    }
    assert(bigIntTestBit(p, firstBit));
    // make it of the form 30k + 1
    p += 31 - (p % 30);
    return p;
}

} // namespace
