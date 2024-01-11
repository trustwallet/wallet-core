// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Solana/LegacyMessage.h"
#include "Solana/AddressLookupTable.h"

namespace TW::Solana {

struct V0Message {
    LegacyMessage msg;
    MessageAddressTableLookup addressTableLookups;
};

}
