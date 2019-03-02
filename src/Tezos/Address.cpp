// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "AddressChecksum.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/sha3.h>

#include <cassert>

using namespace TW::Tezos;

bool Address::isValid(const std::string& string) {
  // TODO: implement
}

Address::Address(const std::string& string) {
  // TODO: implement
}

Address::Address(const std::vector<uint8_t>& data) {
  // TODO: implement
}

Address::Address(const PublicKey& publicKey) {
  // TODO: implement
}

std::string Address::string() const {
    return checksumed(*this);
}
