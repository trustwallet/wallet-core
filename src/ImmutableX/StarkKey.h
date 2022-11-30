// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "uint256.h"
#include <PrivateKey.h>
#include <string>

namespace TW::ImmutableX {

uint256_t hashKeyWithIndex(const std::string& seed, std::size_t index);

std::string grindKey(const std::string& seed);

std::string getPrivateKeyFromSeed(const std::string& seed, const std::string& path);

PrivateKey getPrivateKeyFromEthPrivKey(const PrivateKey& ethPrivKey);

std::string getPrivateKeyFromRawSignature(const std::string& signature, const std::string& ethAddress);

std::string getPublicKeyFromPrivateKey(const std::string& privateKey);

} // namespace TW::ImmutableX
