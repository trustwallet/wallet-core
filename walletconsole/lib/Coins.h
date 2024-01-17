// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace TW::WalletConsole {

using namespace std;

class Coin {
public:
    int c;
    string id;
    string name;
    string symbol;
    int curve; // TWCurve
    int pubKeyType; // TWPublicKeyType
    string derivPath;
};

class Coins {
public:
    Coins(ostream& out) : _out(out) {}
    void coins() const;
    bool findCoin(const string& coin, Coin& coin_out) const;
    void init();
protected:
    void scanCoins();
    int findCoinId(const string& coin) const;
    static int pubKeyTypeFromCurve(int cc);
    ostream& _out;
    map<int, Coin> _coinsByNum;
    map<string, Coin> _coinsById;
    map<string, Coin> _coinsByName;
    map<string, Coin> _coinsBySymbol;
};

} // namespace TW::WalletConsole
