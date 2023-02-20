// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Solana/LegacyMessage.h"
#include "Solana/AddressLookupTable.h"

namespace TW::Solana {

struct V0Message {
    LegacyMessage msg;
    MessageAddressTableLookup addressTableLookups;
};

}
