// Copyright © 2009-2010 Satoshi Nakamoto
// Copyright © 2009-2016 The Bitcoin Core developers
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>

namespace TW::Bitcoin {

/// Amount in satoshis (can be negative)
using Amount = int64_t;

/// One bitcoin in satoshis
inline constexpr Amount coin = 100000000;

/// Maxximum valid amount in satoshis.
inline constexpr Amount maxAmount = 21000000 * coin;

} // namespace TW::Bitcoin
