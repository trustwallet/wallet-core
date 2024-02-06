// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWHRP.h>

#include "Address.h"

namespace TW::MultiversX {

const std::string Address::hrp = HRP_ELROND;

bool Address::isValid(const std::string& string) {
    return Bech32Address::isValid(string, hrp);
}

} // namespace TW::MultiversX
