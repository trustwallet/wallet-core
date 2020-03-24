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
        case TWCoinTypeDigiByte:
        case TWCoinTypeLitecoin:
        case TWCoinTypeMonacoin:
        case TWCoinTypeQtum:
        case TWCoinTypeViacoin:
        case TWCoinTypeBitcoinGold:
            return SegwitAddress::isValid(address, hrp)
                || Address::isValid(address, {{p2pkh}, {p2sh}});

        case TWCoinTypeBitcoinCash:
            return CashAddress::isValid(address)
                || Address::isValid(address, {{p2pkh}, {p2sh}});

        case TWCoinTypeDash:
        case TWCoinTypeDogecoin:
        case TWCoinTypeRavencoin:
        case TWCoinTypeZcoin:
        default:
            return Address::isValid(address, {{p2pkh}, {p2sh}});
    }
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    switch (coin) {
        case TWCoinTypeBitcoinCash:
            // normalized with bitcoincash: prefix
            if (CashAddress::isValid(address)) {
                return CashAddress(address).string();
            } else {
                return std::string(address);
            }

        default:
            // no change
            return address;
    }
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const {
    switch (coin) {
        case TWCoinTypeBitcoin:
        case TWCoinTypeDigiByte:
        case TWCoinTypeLitecoin:
        case TWCoinTypeViacoin:
        case TWCoinTypeBitcoinGold:
            return SegwitAddress(publicKey, 0, hrp).string();

        case TWCoinTypeBitcoinCash:
            return CashAddress(publicKey).string();

        case TWCoinTypeDash:
        case TWCoinTypeDogecoin:
        case TWCoinTypeMonacoin:
        case TWCoinTypeQtum:
        case TWCoinTypeRavencoin:
        case TWCoinTypeZcoin:
        default:
            return Address(publicKey, p2pkh).string();
    }
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}
