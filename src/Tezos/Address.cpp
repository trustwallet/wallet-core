// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "HexCoding.h"
#include "TWTezosForger.h"

using namespace TW;
using namespace TW::Tezos;

Address::Address(const std::string& pkey_hash) {
  public_key_hash = pkey_hash;
}

std::string Address::forge() const {
  std::string result = "";
  if (public_key_hash[0] == 'K') {
    size_t prefixLength = 3;
    uint8_t prefix[3] = {2, 90, 121};
    size_t capacity = 128;
    uint8_t decoded[capacity];

    int decodedLength = checkDecodeAndDropPrefix(public_key_hash, prefixLength, prefix, decoded);
    result += "01";
    result += TW::hex(decoded, decoded + decodedLength);
    result += "00";
  } else {
    // tz1 address
    result += "00";
    result += forgePublicKeyHash(public_key_hash);
  }
  return result;
}
