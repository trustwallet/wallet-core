// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <vector>
#include <map>

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
};

class Coins {
public:
    Coins() {}
    void coins() const;
    //bool coin(const string& id) const;
    bool findCoin(const string& coin, Coin& coin_out) const;
    void init();
protected:
    void scanCoinRange(int from, int to);
    int findCoinId(const string& coin) const;
    static int pubKeyTypeFromCurve(int cc);
    map<int, Coin> _coinsByNum;
    map<string, Coin> _coinsById;
    map<string, Coin> _coinsByName;
    map<string, Coin> _coinsBySymbol;
};

} // namespace TW::WalletConsole
