// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../PublicKey.h"
#include <string>

using namespace TW;

std::string forgeBool(bool input);
std::string forgePublicKeyHash(const std::string &publicKeyHash);
std::string forgePublicKey(PublicKey publicKey);
