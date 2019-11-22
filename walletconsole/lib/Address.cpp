// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "WalletConsole.h"
#include "Coin.h"
#include "Data.h"
#include "PrivateKey.h"
#include "HexCoding.h"

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;

bool Address::addrpub(const string& coinid, const string& pubkey_in, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    Data pubDat;
    try {
        pubDat = parse_hex(pubkey_in);
    } catch (exception& ex) {
        cout << "Error: could not parse public key data" << endl;
        return false; 
    }
    TWCoinType ctype = (TWCoinType)coin.c;
    PublicKey pubkey = PublicKey(pubDat, (TWPublicKeyType)coin.pubKeyType);
    res = TW::deriveAddress(ctype, pubkey);
    return true;
}

bool Address::addrpri(const string& coinid, const string& prikey_in, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    Data priDat;
    try {
        priDat = parse_hex(prikey_in);
    } catch (exception& ex) {
        cout << "Error: could not parse private key data" << endl;
        return false; 
    }
    TWCoinType ctype = (TWCoinType)coin.c;
    PrivateKey prikey = PrivateKey(priDat);
    res = TW::deriveAddress(ctype, prikey);
    return true;
}

bool Address::addr(const string& coinid, const string& addrStr, string& res) {
    Coin coin;
    if (!_coins.findCoin(coinid, coin)) { return false; }
    TWCoinType ctype = (TWCoinType)coin.c;
    if (!TW::validateAddress(ctype, addrStr)) {
        cout << "Address is not a valid " << coin.name << " address! " << addrStr << endl;
        return false;
    }
    cout << "Address is a valid " << coin.name << " address:  " << addrStr << endl;
    return false;
}

} // namespace TW::WalletConsole
