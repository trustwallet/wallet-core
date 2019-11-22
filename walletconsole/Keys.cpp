// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Keys.h"

#include "WalletConsole.h"
#include "Data.h"
#include "PrivateKey.h"
#include "HexCoding.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <stdlib.h>
#include <time.h>

namespace TW::WalletConsole {

using namespace std;

Keys::Keys(const Coins& coins) : _coins(coins) {
    // init pseudo-random
    ::srand(::time(NULL));
}

bool Keys::newkey(string& res) {
    int n = 32;
    Data k = Data(n);
    for (int i = 0; i < n; ++i) {
        k[i] = ::rand() % 256;
    }
    res = hex(k);
    return true;
}

bool Keys::pubpri(const string& coinid, const string& p, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    Data privDat;
    try {
        privDat = parse_hex(p);
    } catch (exception& ex) {
        cout << "Error: could not parse private key data" << endl;
        return false; 
    }
    auto priv = PrivateKey(privDat);
    auto pub = priv.getPublicKey((TWPublicKeyType)coin.pubKeyType);
    res = hex(pub.bytes);
    return true;
}

bool Keys::pripub(const string& p, string& res) {
    cout << "Not yet implemented! :)" << endl;
    return false;
}

} // namespace TW::WalletConsole
