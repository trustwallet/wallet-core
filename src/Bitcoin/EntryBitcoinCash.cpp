// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EntryBitcoinCash.h"

#include "Address.h"
#include "CashAddress.h"

using namespace TW::Bitcoin;
using namespace std;

bool EntryBitcoinCash::validateAddress(TWCoinType coin, const string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const {
    return CashAddress::isValid(address)
        || Address::isValid(address, {{p2pkh}, {p2sh}});
}

string EntryBitcoinCash::normalizeAddress(TWCoinType coin, const string& address) const {
    // normalized with bitcoincash: prefix
    if (CashAddress::isValid(address)) {
        return CashAddress(address).string();
    } else {
        return std::string(address);
    }
}

string EntryBitcoinCash::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const {
    return CashAddress(publicKey).string();
}
