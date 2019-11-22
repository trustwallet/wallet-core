// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Coins.h"
#include "HexCoding.h"
#include "Data.h"

#include <string>

namespace TW::WalletConsole {

using namespace std;

class Keys {
private:
    const Coins& _coins;
public:
    Keys(const Coins& coins);
    bool newkey(string& res);
    /// Public key from private key, ED25519
    bool pubpri(const string& coinid, const string& p, string& res);
    bool pripub(const string& p, string& res);
};

} // namespace TW::WalletConsole
