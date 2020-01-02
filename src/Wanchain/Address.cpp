// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../Ethereum/AddressChecksum.h"

#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/sha3.h>

#include <cassert>

using namespace TW::Wanchain;

std::string Address::string() const {
    return Ethereum::checksumed(*this, Ethereum::ChecksumType::wanchain);
}
