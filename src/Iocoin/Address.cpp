// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Base58.h"
#include <TrezorCrypto/ecdsa.h>

#include <cassert>

using namespace TW::Iocoin;

Address::Address(const PublicKey& publicKey, uint8_t prefix) {
    if (publicKey.type() != PublicKeyType::secp256k1) {
        throw std::invalid_argument("Iocoin::Address needs a compressed SECP256k1 public key.");
    }
    bytes[0] = prefix;
   const auto data = publicKey.hash({prefix}, Hash::sha256ripemd); 

   std::copy(data.begin(), data.end(), this->bytes.begin());
}

