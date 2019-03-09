// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"

#include "Bitcoin/Address.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/Bech32Address.h"
#include "Ethereum/Address.h"
#include "Icon/Address.h"
#include "Nimiq/Address.h"
#include "Ripple/Address.h"
#include "Tendermint/Address.h"
#include "Tezos/Address.h"
#include "Tron/Address.h"
#include "Zcash/TAddress.h"

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>

using namespace TW;

std::string TW::loadAddress(TWCoinType coin, const Data& data) {
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

    case TWCoinTypeTron:
        return Tron::Address(data).string();

    case TWCoinTypeDash:
    case TWCoinTypeZcoin:
        return Bitcoin::Address(data).string();

    case TWCoinTypeZcash:
        return Zcash::TAddress(data).string();

    case TWCoinTypeTezos:
        return "";
    }
}

bool TW::validateAddress(TWCoinType coin, const std::string& string) {
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
        return Tron::Address::isValid(string);

    case TWCoinTypeZcoin:
        return Bitcoin::Address::isValid(string, {TWP2PKHPrefixZcoin, TWP2SHPrefixZcoin});

    case TWCoinTypeZcash:
        return Zcash::TAddress::isValid(string, {TWP2PKHPrefixZcashT, TWP2SHPrefixZcashT});
    }
}

TWPurpose TW::purpose(TWCoinType coin) {
    switch(coin) {
    case TWCoinTypeBinance:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeCallisto:
    case TWCoinTypeDash:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypeICON:
    case TWCoinTypeNimiq:
    case TWCoinTypePoa:
    case TWCoinTypeRipple:
    case TWCoinTypeTezos:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        return TWPurposeBIP44;
    case TWCoinTypeBitcoin:
    case TWCoinTypeLitecoin:
        return TWPurposeBIP84;
    }
}

TWCurve TW::curve(TWCoinType coin) {
    switch(coin) {
    case TWCoinTypeBinance:
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeCallisto:
    case TWCoinTypeDash:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypeICON:
    case TWCoinTypeLitecoin:
    case TWCoinTypePoa:
    case TWCoinTypeRipple:
    case TWCoinTypeTezos:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        return TWCurveSECP256k1;
    case TWCoinTypeNimiq:
        return TWCurveEd25519;
    }
}

DerivationPath TW::derivationPath(TWCoinType coin) {
    switch (coin) {
    case TWCoinTypeBinance:
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeCallisto:
    case TWCoinTypeDash:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypeICON:
    case TWCoinTypeLitecoin:
    case TWCoinTypeNimiq:
    case TWCoinTypePoa:
    case TWCoinTypeRipple:
    case TWCoinTypeTezos:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        return DerivationPath(purpose(coin), coin, 0, 0, 0);
    }
}

std::string TW::deriveAddress(TWCoinType coin, const PrivateKey& privateKey) {
    switch (coin) {
    case TWCoinTypeBinance:
        return Tendermint::Address(HRP_BINANCE, privateKey.getPublicKey(PublicKeyType::secp256k1)).string();

    case TWCoinTypeBitcoin:
        return Bitcoin::Bech32Address(privateKey.getPublicKey(PublicKeyType::secp256k1), 0, HRP_BITCOIN).string();

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress(privateKey.getPublicKey(PublicKeyType::secp256k1)).string();

    case TWCoinTypeDash:
        return Bitcoin::Address(privateKey.getPublicKey(PublicKeyType::secp256k1), TWP2PKHPrefixDash).string();

    case TWCoinTypeCallisto:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypePoa:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
        return Ethereum::Address(privateKey.getPublicKey(PublicKeyType::secp256k1Extended)).string();

    case TWCoinTypeICON:
        return Icon::Address(privateKey.getPublicKey(PublicKeyType::secp256k1), TWIconAddressTypeAddress).string();

    case TWCoinTypeLitecoin:
        return Bitcoin::Bech32Address(privateKey.getPublicKey(PublicKeyType::secp256k1), 0, HRP_LITECOIN).string();

    case TWCoinTypeNimiq:
        return Nimiq::Address(privateKey.getPublicKey(PublicKeyType::ed25519)).string();

    case TWCoinTypeRipple:
        return Ripple::Address(privateKey.getPublicKey(PublicKeyType::secp256k1)).string();

    case TWCoinTypeTezos:
        return Tezos::Address(privateKey.getPublicKey(PublicKeyType::secp256k1)).string();

    case TWCoinTypeTron:
        return Tron::Address(privateKey.getPublicKey(PublicKeyType::secp256k1)).string();

    case TWCoinTypeZcash:
        return Zcash::TAddress(privateKey.getPublicKey(PublicKeyType::secp256k1), TWP2SHPrefixZcashT).string();

    case TWCoinTypeZcoin:
        return Bitcoin::Address(privateKey.getPublicKey(PublicKeyType::secp256k1), TWP2PKHPrefixZcoin).string();
    }
}
