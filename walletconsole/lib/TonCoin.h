// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TON/Address.h"
#include "TON/Cell.h"
#include "TON/Signer.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include "HexCoding.h"

#include <string>
#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;
using namespace TW;
using namespace TW::TON;

class TonCoin {
public:
    static bool tonInitMsg(const string& privkey, string& res) {
        PrivateKey privv = PrivateKey(parse_hex(privkey));
        Data extMsg = Signer::buildInitMessage(privv);
        res = hex(extMsg);
        return true;
    }
};

} // namespace TW::WalletConsole
