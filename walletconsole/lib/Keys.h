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
#include <iostream>

namespace TW::WalletConsole {

using namespace std;

class Keys {
private:
    ostream& _out;
    const Coins& _coins;
    string _currentMnemonic;
    
public:
    Keys(ostream& out, const Coins& coins);
    bool newkey(string& res);
    /// Public key from private key, ED25519
    bool pubpri(const string& coinid, const string& p, string& res);
    bool pripub(const string& p, string& res);
    string getMnemo() const { return _currentMnemonic; }
    /// Set given menmonic; list of separate words
    void setmnemo(const vector<string>& param);
    /// Generate and store new mnemonic
    bool newmnemo(const string& param1, string& res);
    /// Print seed of current mnemonic
    bool printseed(string& res);
    /// Print out the current mnemonic
    bool printmnemo(string& res);
};

} // namespace TW::WalletConsole
