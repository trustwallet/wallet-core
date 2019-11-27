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
#include "HDWallet.h"

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;

Keys::Keys(ostream& out, const Coins& coins) : _out(out), _coins(coins) {
    // init a random mnemonic
    HDWallet newwall(128, "");
    _currentMnemonic = newwall.mnemonic;
}

bool Keys::newKey(string& res) {
    // Create a new private key by creating a new HDWallet and deriving from it
    HDWallet newWallet(256, "");
    DerivationPath dummyDerivation("m/84'/0'/0'/0/0");
    PrivateKey key = newWallet.getKey(dummyDerivation);
    res = hex(key.bytes);
    return true;
}

bool Keys::pubPri(const string& coinid, const string& p, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    Data privDat;
    try {
        privDat = parse_hex(p);
    } catch (exception& ex) {
        _out << "Error: could not parse private key data" << endl;
        return false; 
    }
    auto priv = PrivateKey(privDat);
    auto pub = priv.getPublicKey((TWPublicKeyType)coin.pubKeyType);
    res = hex(pub.bytes);
    return true;
}

bool Keys::priPub(const string& p, string& res) {
    _out << "Not yet implemented! :)" << endl;
    return false;
}

void Keys::setMnemonic(const vector<string>& param) {
    if (param.size() < 1 + 12) {
        _out << "Error: at least 12 words are needed for the mnemonic!" << endl;
        return;
    }
    // concatenate
    string mnem = "";
    for (int i = 1; i < param.size(); ++i) {
        if (i > 1) mnem += " ";
        mnem += param[i]; 
    }

    // verify mnemonic
    if (!HDWallet::isValid(mnem)) {
        _out << "Not a valid mnemonic: " << mnem << endl;
        return;
    }

    // store
    _currentMnemonic = mnem;
    _out << "Mnemonic set (" << param.size() - 1 << " words)." << endl;
}

bool Keys::newMnemonic(const string& param1, string& res) {
    int strength = stoi(param1);
    if (strength < 128 || strength > 256 || (strength % 32 != 0)) {
        _out << "Error: strength must be between 128 and 256, and multiple of 32" << endl;
        return false;
    }
    HDWallet newwall(strength, "");
    if (newwall.mnemonic.length() == 0) {
        _out << "Error: no mnemonic generated." << endl;
        return false;
    }
    // store
    _currentMnemonic = newwall.mnemonic;
    res = _currentMnemonic;
    _out << "New mnemonic set." << endl;
    return false;
}

bool Keys::dumpSeed(string& res) {
    if (_currentMnemonic.length() == 0) {
        _out << "Error: no mnemonic set.  Use setMnemonic." << endl;
        return false;
    }
    HDWallet wallet(_currentMnemonic, "");
    string seedHex = hex(wallet.seed);
    res = seedHex;
    return true;
}

bool Keys::dumpMnemonic(string& res) {
    if (_currentMnemonic.length() == 0) {
        _out << "Error: no mnemonic set.  Use setMnemonic." << endl;
        return false;
    }
    res = _currentMnemonic;
    return true;
}

bool Keys::dumpDP(const string& coinid, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    res = coin.derivPath;
    return true;
}

bool Keys::priDP(const string& coinid, const string& dp, string& res) {
    // coin
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }

    // mnemo
    if (_currentMnemonic.length() == 0) {
        _out << "Error: no mnemonic set.  Use setMnemonic." << endl;
        return false;
    }

    // derivation path
    string dp2 = dp;
    if (dp2.length() == 0) {
        // missing dp, use default
        dp2 = coin.derivPath;
    }
    DerivationPath dp3(dp2);
    _out << "Using derivation path \"" << dp2 << "\" for coin " << coin.name << endl;

    HDWallet wallet(_currentMnemonic, "");
    PrivateKey priKey = wallet.getKey(dp3);

    res = hex(priKey.bytes);
    return true;
}

} // namespace TW::WalletConsole
