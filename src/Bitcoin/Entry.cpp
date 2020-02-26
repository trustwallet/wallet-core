// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "CashAddress.h"
#include "SegwitAddress.h"
#include "Signer.h"

using namespace TW::Bitcoin;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const {
    switch (coin) {
        case TWCoinTypeBitcoin:
            return SegwitAddress::isValid(address, hrp)
                || Address::isValid(address, {{p2pkh}, {p2sh}});

        case TWCoinTypeBitcoinCash:
            return CashAddress::isValid(address)
                || Address::isValid(address, {{p2pkh}, {p2sh}});

        default:
            assert(false);
            return false;
    }
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    switch (coin) {
        case TWCoinTypeBitcoin:
            return address;

        case TWCoinTypeBitcoinCash:
            // normalized with bitcoincash: prefix
            if (CashAddress::isValid(address)) {
                return CashAddress(address).string();
            } else {
                return std::string(address);
            }

        default:
            assert(false);
            return "";
    }
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char* hrp) const {
    switch (coin) {
        case TWCoinTypeBitcoin:
            return SegwitAddress(publicKey, 0, hrp).string();

        case TWCoinTypeBitcoinCash:
            return CashAddress(publicKey).string();

        default:
            assert(false);
            return "";
    }
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    AnySignTempl<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}
