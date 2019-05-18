// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../Hash.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::ARK;

Address::Address(const PublicKey &publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Ark::Address needs a compressed SECP256k1 public key.");
    }
    const auto data =
        publicKey.hash({Address::prefix}, static_cast<Data (*)(const byte*, const byte*)>(Hash::ripemd), false);
    std::copy(data.begin(), data.end(), bytes.begin());
}
