// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Keys.h"
#include "Coins.h"
#include "HexCoding.h"
#include "Data.h"

#include <string>
#include <iostream>

namespace TW::WalletConsole {

using namespace std;

class Address {
private:
    ostream& _out;
    const Coins& _coins;
    const Keys& _keys;

public:
    Address(ostream& out, const Coins& coins, const Keys& keys) : _out(out), _coins(coins), _keys(keys) {}
    bool addrpub(const string& coinid, const string& pubkey, string& res);
    bool addrpri(const string& coinid, const string& prikey, string& res);
    bool addr(const string& coinid, const string& addrStr, string& res);
    /// Derive a default address, using default coin and current mnemonic
    bool addrdef(const string& coinid, string& res);
    /// Derive a new address with the given derivation path
    bool addrdp(const string& coinid, const string& derivPath, string& res);
};

} // namespace TW::WalletConsole
