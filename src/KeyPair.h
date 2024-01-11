// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <tuple>
#include "PrivateKey.h"
#include "PublicKey.h"

namespace TW {

typedef std::tuple<PrivateKey, PublicKey> KeyPair;

} // namespace
