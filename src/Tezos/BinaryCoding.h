// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

int checkDecodeAndDropPrefix(const std::string& input, size_t prefixLength, uint8_t *prefix, uint8_t *output);
std::string forgeBool(bool input);
std::string forgePublicKeyHash(const std::string &publicKeyHash);
std::string forgeAddress(const std::string address);
std::string forgePublicKey(std::string publicKey);
