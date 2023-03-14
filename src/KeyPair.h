// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <tuple>
#include "PrivateKey.h"
#include "PublicKey.h"

namespace TW {

typedef std::tuple<PrivateKey, PublicKey> KeyPair;

} // namespace
