// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <Solana/Address.h>

namespace TW::Solana {

struct AddressLookupTable {
    Solana::Address key;
    std::vector<Solana::Address> addresses;
};

using MessageAddressTableLookup = std::vector<AddressLookupTable>;

}
