// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <Solana/Address.h>

namespace TW::Solana {

struct AddressLookupTable {
    Solana::Address key;
    std::vector<Solana::Address> addresses;
};

using MessageAddressTableLookup = std::vector<AddressLookupTable>;

}
