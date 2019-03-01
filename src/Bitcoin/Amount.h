// Copyright © 2009-2010 Satoshi Nakamoto
// Copyright © 2009-2016 The Bitcoin Core developers
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <stdint.h>

namespace TW {
namespace Bitcoin {

/// Amount in satoshis (can be negative)
typedef int64_t Amount;

/// One bitcoin in satoshis
static const Amount coin = 100000000;

/// Maxximum valid amount in satoshis.
static const Amount maxAmount = 21000000 * coin;

/// Detemines if the provided value is a valid amount.
inline bool isValidAmount(const Amount& amount) {
    return (amount >= 0 && amount <= maxAmount);
}

}} // namespace
