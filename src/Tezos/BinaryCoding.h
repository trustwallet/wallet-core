// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

using namespace TW;

int base58CheckDecodePrefix(const std::string& input, size_t prefixLength, uint8_t *prefix, uint8_t *output);
PublicKey parsePublicKey(std::string publicKey);
std::string forgeBool(bool input);
std::string forgePublicKeyHash(const std::string &publicKeyHash);
std::string forgeAddress(const std::string address);
std::string forgePublicKey(PublicKey publicKey);
