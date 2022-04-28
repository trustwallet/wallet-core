// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "BigInt.h"

namespace TW::Arweave {

/// Generate a good starting point prime from num (result will be larger than num, and have at least bits bits).
BigInt primeCandidateFromInt(const BigInt& num, int bits);

} // namespace
