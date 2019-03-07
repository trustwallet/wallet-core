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
#include "Zcash/TAddress.h"

#include <TrustWalletCore/TWHRP.h>

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
    case TWCoinTypeTezos:
    case TWCoinTypeTron:
    case TWCoinTypeZcoin:
        return Bitcoin::Address(data).string();

    case TWCoinTypeZcash:
        return Zcash::TAddress(data).string();

    case TWCoinTypeEOS:
        return "";
    }
}
