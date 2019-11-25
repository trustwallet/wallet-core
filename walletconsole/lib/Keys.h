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
    bool newKey(string& res);
    /// Public key from private key, ED25519
    bool pubPri(const string& coinid, const string& p, string& res);
    bool priPub(const string& p, string& res);
    string getMnemo() const { return _currentMnemonic; }
    /// Set given menmonic; list of separate words
    void setMnemo(const vector<string>& param);
    /// Generate and store new mnemonic
    bool newMnemo(const string& param1, string& res);
    /// Dump seed of current mnemonic
    bool dumpSeed(string& res);
    /// Dump the current mnemonic
    bool dumpMnemo(string& res);
    /// Dump default derivation path of coin
    bool dumpDP(const string& coinid, string& res);
    /// Derive a private key from the menmonic with derivation path
    bool priDP(const string& coinid, const string& dp, string& res);
};

} // namespace TW::WalletConsole
