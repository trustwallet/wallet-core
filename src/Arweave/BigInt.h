// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <Data.h>

#include <boost/multiprecision/cpp_int.hpp> 

namespace TW::Arweave {

using BigInt = boost::multiprecision::cpp_int;

/// Build number from bytes
BigInt bigIntFromData(const Data& data);

/// First byte is sign, ignored
BigInt bigIntFromDataWithSign(const Data& data);

/// Check one bit of the number
bool bigIntTestBit(const BigInt& num, int bit);

} // namespace
