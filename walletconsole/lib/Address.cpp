// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"

#include "WalletConsole.h"
#include "Coin.h"
#include "Data.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "HDWallet.h"

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;

bool Address::addrPub(const string& coinid, const string& pubkey_in, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    Data pubDat;
    try {
        pubDat = parse_hex(pubkey_in);
    } catch (exception& ex) {
        _out << "Error: could not parse public key data" << endl;
        return false;
    }
    auto ctype = (TWCoinType)coin.c;
    PublicKey pubKey = PublicKey(pubDat, (TWPublicKeyType)coin.pubKeyType);
    res = TW::deriveAddress(ctype, pubKey);
    return true;
}

bool Address::addrPri(const string& coinid, const string& prikey_in, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    Data priDat;
    try {
        priDat = parse_hex(prikey_in);
    } catch (exception& ex) {
        _out << "Error: could not parse private key data" << endl;
        return false;
    }
    auto ctype = (TWCoinType)coin.c;
    PrivateKey priKey = PrivateKey(priDat);
    res = TW::deriveAddress(ctype, priKey);
    return true;
}

bool Address::addr(const string& coinid, const string& addrStr, [[maybe_unused]] string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    auto ctype = (TWCoinType)coin.c;
    if (!TW::validateAddress(ctype, addrStr)) {
        _out << "Address is not a valid " << coin.name << " address! " << addrStr << endl;
        return false;
    }
    _out << "Address is a valid " << coin.name << " address:  " << addrStr << endl;
    return false;
}

bool Address::addrDefault(const string& coinid, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    auto ctype = (TWCoinType)coin.c;
    string mnemo = _keys.getMnemo();
    assert(mnemo.length() > 0); // a mnemonic is always set
    HDWallet wallet(mnemo, "");

    string addr = wallet.deriveAddress(ctype);

    res = addr;
    return true;
}

bool Address::deriveFromPath(const string& coinid, const string& derivPath, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    auto ctype = (TWCoinType)coin.c;

    DerivationPath dp(derivPath);
    // get the private key
    string mnemo = _keys.getMnemo();
    assert(mnemo.length() > 0); // a mnemonic is always set
    HDWallet wallet(mnemo, "");
    PrivateKey priKey = wallet.getKey(ctype, dp);

    // derive address
    res = TW::deriveAddress(ctype, priKey);
    return true;
}

bool Address::deriveFromXpubIndex(const string& coinid, const string& xpub, const string& accountIndex, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    auto ctype = (TWCoinType)coin.c;

    int index = std::stoi(accountIndex);

    // Derivation path: use default, but only elements 4&5 (change&address) are used
    DerivationPath dp(coin.derivPath);
    dp.setChange(0);
    dp.setAddress(index);

    const auto publicKey = HDWallet<>::getPublicKeyFromExtended(xpub, ctype, dp);
    if (!publicKey) { return false; }
    res = TW::deriveAddress(ctype, publicKey.value());
    return true;
}

} // namespace TW::WalletConsole
