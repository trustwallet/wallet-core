// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Keys.h"

#include "WalletConsole.h"
#include "Data.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "HDWallet.h"
#include "Mnemonic.h"
#include "Coin.h"

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;

Keys::Keys(ostream& out, const Coins& coins) : _out(out), _coins(coins) {
    // init a random mnemonic
    HDWallet newwall(128, "");
    _currentMnemonic = newwall.getMnemonic();
}

void privateKeyToResult(const PrivateKey& priKey, string& res_out) {
    // take the key, but may need to take extension as well
    res_out = hex(priKey.bytes);
}

bool Keys::newKey(const string& coinid, string& res) {
    // Create a new private key by creating a new HDWallet and deriving from it
    // Use coin-specific derivation path, so that PK can be coin-specific (e.g. longer for Cardano)
    // coin
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }

    HDWallet newWallet(256, "");

    DerivationPath derivationPath = DerivationPath(coin.derivPath);
    PrivateKey key = newWallet.getKey(TWCoinType(coin.c), derivationPath);
    privateKeyToResult(key, res);
    return true;
}

bool Keys::pubPri(const string& coinid, const string& p, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    Data privDat;
    try {
        privDat = parse_hex(p);
        auto priv = PrivateKey(privDat);
        auto pub = priv.getPublicKey((TWPublicKeyType)coin.pubKeyType);
        res = hex(pub.bytes);
        _out << "Public key created, type " << (int)coin.pubKeyType << ", length " << pub.bytes.size() << endl;
        return true;
    } catch (exception& ex) {
        _out << "Error: " << ex.what() << endl;
        return false; 
    }
}

bool Keys::priPub([[maybe_unused]] const string& p, [[maybe_unused]] string& res) {
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
    for (auto i = 1ul; i < param.size(); ++i) {
        if (i > 1) mnem += " ";
        mnem += param[i]; 
    }

    // verify mnemonic
    if (!Mnemonic::isValid(mnem)) {
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
    if (newwall.getMnemonic().length() == 0) {
        _out << "Error: no mnemonic generated." << endl;
        return false;
    }
    // store
    _currentMnemonic = newwall.getMnemonic();
    res = _currentMnemonic;
    _out << "New mnemonic set." << endl;
    return false;
}

bool Keys::dumpSeed(string& res) {
    assert(_currentMnemonic.length() > 0); // a mnemonic is always set
    HDWallet wallet(_currentMnemonic, "");
    string seedHex = hex(wallet.getSeed());
    res = seedHex;
    return true;
}

bool Keys::dumpMnemonic(string& res) {
    assert(_currentMnemonic.length() > 0); // a mnemonic is always set
    res = _currentMnemonic;
    return true;
}

bool Keys::dumpDP(const string& coinid, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    res = coin.derivPath;
    return true;
}

bool Keys::dumpXpub(const string& coinid, string& res) {
    assert(_currentMnemonic.length() > 0); // a mnemonic is always set
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    TWCoinType ctype = (TWCoinType)coin.c;
    TWPurpose purpose = TW::purpose(ctype);
    TWHDVersion xpubVersion = TW::xpubVersion(ctype);
    HDWallet wallet(_currentMnemonic, "");
    string xpub = wallet.getExtendedPublicKey(purpose, ctype, xpubVersion);
    res = xpub;
    return true;
}

bool Keys::priDP(const string& coinid, const string& dp, string& res) {
    // coin
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }

    // mnemo
    assert(_currentMnemonic.length() > 0); // a mnemonic is always set

    // derivation path
    string dp2 = dp;
    if (dp2.length() == 0) {
        // missing dp, use default
        dp2 = coin.derivPath;
    }
    DerivationPath dp3(dp2);
    _out << "Using derivation path \"" << dp2 << "\" for coin " << coin.name << endl;

    HDWallet wallet(_currentMnemonic, "");
    PrivateKey priKey = wallet.getKey(TWCoinType(coin.c), dp3);

    privateKeyToResult(priKey, res);
    return true;
}

} // namespace TW::WalletConsole
