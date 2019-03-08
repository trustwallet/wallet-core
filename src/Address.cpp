// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "Bitcoin/Address.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/Bech32Address.h"
#include "Ethereum/Address.h"
#include "Icon/Address.h"
#include "Nimiq/Address.h"
#include "Ripple/Address.h"
#include "Tendermint/Address.h"
#include "Tezos/Address.h"
#include "Zcash/TAddress.h"

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>

using namespace TW;

std::string TW::loadAddress(const Data& data, TWCoinType coin) {
    switch (coin) {
    case TWCoinTypeBinance:
        return Tendermint::Address(HRP_BINANCE, data).string();

    case TWCoinTypeBitcoin:
        return Bitcoin::Bech32Address(HRP_BITCOIN, 0, data).string();

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress(data).string();

    case TWCoinTypeCallisto:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypePoa:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
        return Ethereum::Address(data).string();

    case TWCoinTypeICON:
        return Icon::Address(data, TWIconAddressTypeAddress).string();

    case TWCoinTypeLitecoin:
        return Bitcoin::Bech32Address(HRP_LITECOIN, 0, data).string();

    case TWCoinTypeNimiq:
        return Nimiq::Address(data).string();

    case TWCoinTypeRipple:
        return Ripple::Address(data).string();

    case TWCoinTypeDash:
    case TWCoinTypeTron:
    case TWCoinTypeZcoin:
        return Bitcoin::Address(data).string();

    case TWCoinTypeZcash:
        return Zcash::TAddress(data).string();

    case TWCoinTypeTezos:
    case TWCoinTypeEOS:
        return "";
    }
}

bool TW::validate(const std::string& string, TWCoinType coin) {
    switch (coin) {
    case TWCoinTypeBinance:
        return Tendermint::Address::isValid(string, HRP_BINANCE) || Tendermint::Address::isValid(string, HRP_BINANCE_TEST);

    case TWCoinTypeBitcoin:
        return Bitcoin::Bech32Address::isValid(string, HRP_BITCOIN) || Bitcoin::Address::isValid(string, {TWP2PKHPrefixBitcoin, TWP2SHPrefixBitcoin});

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress::isValid(string) || Bitcoin::Address::isValid(string, {TWP2PKHPrefixBitcoin, TWP2SHPrefixBitcoin});

    case TWCoinTypeCallisto:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypePoa:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
        return Ethereum::Address::isValid(string);

    case TWCoinTypeICON:
        return Icon::Address::isValid(string);

    case TWCoinTypeLitecoin:
        return Bitcoin::Bech32Address::isValid(string, HRP_LITECOIN) || Bitcoin::Address::isValid(string, {TWP2PKHPrefixLitecoin, TWP2SHPrefixLitecoin});

    case TWCoinTypeNimiq:
        return Nimiq::Address::isValid(string);

    case TWCoinTypeRipple:
        return Ripple::Address::isValid(string);

    case TWCoinTypeDash:
        return Bitcoin::Address::isValid(string, {TWP2PKHPrefixDash, TWP2SHPrefixDash});

    case TWCoinTypeTezos:
        return Tezos::Address::isValid(string);

    case TWCoinTypeTron:
        return Bitcoin::Address::isValid(string, {0x41});

    case TWCoinTypeZcoin:
        return Bitcoin::Address::isValid(string, {TWP2PKHPrefixZcoin, TWP2SHPrefixZcoin});

    case TWCoinTypeZcash:
        return Zcash::TAddress::isValid(string, {TWP2PKHPrefixZcashT, TWP2SHPrefixZcashT});

    case TWCoinTypeEOS:
        return false;
    }
}
