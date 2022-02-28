// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../PublicKey.h"
#include "../proto/Tezos.pb.h"

#include <string>

using namespace TW;
using namespace TW::Tezos::Proto;

Data forgeBool(bool input);
Data forgeOperation(const Operation& operation);
Data forgePublicKeyHash(const std::string& publicKeyHash);
Data forgePublicKey(PublicKey publicKey);
Data forgeZarith(uint64_t input);
