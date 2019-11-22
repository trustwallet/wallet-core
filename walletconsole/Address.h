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

class Address {
private:
    const Coins& _coins;
public:
    Address(const Coins& coins) : _coins(coins) {}
    bool addrpub(const string& coinid, const string& pubkey, string& res);
    bool addrpri(const string& coinid, const string& prikey, string& res);
    bool addr(const string& coinid, const string& addrStr, string& res);
};

} // namespace TW::WalletConsole
