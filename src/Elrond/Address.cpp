// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHRP.h>

#include "Address.h"

using namespace TW::Elrond;

const std::string Address::hrp = HRP_ELROND;

bool Address::isValid(const std::string& string) {
    return Bech32Address::isValid(string, hrp);
}
