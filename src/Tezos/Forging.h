// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../PublicKey.h"
#include "../proto/Tezos.pb.h"

#include <string>

using namespace TW;

Data forgeBool(bool input);
Data forgeOperation(const TW::Tezos::Proto::Operation& operation);
Data forgePublicKeyHash(const std::string& publicKeyHash);
Data forgePublicKey(PublicKey publicKey);
Data forgeZarith(uint64_t input);
