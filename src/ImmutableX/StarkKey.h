// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "uint256.h"
#include <PrivateKey.h>
#include <DerivationPath.h>
#include <string>

namespace TW::ImmutableX {

uint256_t hashKeyWithIndex(const Data& seed, std::size_t index);

std::string grindKey(const Data& seed);

PrivateKey getPrivateKeyFromSeed(const std::string& seed, const DerivationPath& path);

PrivateKey getPrivateKeyFromEthPrivKey(const PrivateKey& ethPrivKey);

PrivateKey getPrivateKeyFromRawSignature(const Data& signature, const DerivationPath& derivationPath);

} // namespace TW::ImmutableX
