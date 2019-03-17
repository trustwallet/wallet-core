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
#include "Aion/Address.h"
#include "Ripple/Address.h"
#include "Tendermint/Address.h"
#include "Tezos/Address.h"
#include "Tron/Address.h"
#include "Zcash/TAddress.h"
#include "Stellar/Address.h"
#include "NEO/Address.h"

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>

using namespace TW;

std::string TW::loadAddress(TWCoinType coin, const Data& data) {
    switch (coin) {
    case TWCoinTypeAion:
        return Aion::Address(data).string();

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
    case TWCoinTypeXDai:
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

    case TWCoinTypeStellar:
        return Stellar::Address(data).string();

    case TWCoinTypeTezos:
        return Tezos::Address(data).string();
    case TWCoinTypeNEO:
        return NEO::Address(data).string();
    }
}

bool TW::validateAddress(TWCoinType coin, const std::string& string) {
    switch (coin) {
    case TWCoinTypeAion:
        return Aion::Address::isValid(string);

    case TWCoinTypeBinance:
        return Tendermint::Address::isValid(string, HRP_BINANCE) || Tendermint::Address::isValid(string, HRP_BINANCE_TEST);

    case TWCoinTypeBitcoin:
        return Bitcoin::Bech32Address::isValid(string, HRP_BITCOIN) || Bitcoin::Address::isValid(string, {TWP2PKHPrefixBitcoin, TWP2SHPrefixBitcoin});

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress::isValid(string) || Bitcoin::Address::isValid(string, {TWP2PKHPrefixBitcoin, TWP2SHPrefixBitcoin});

    case TWCoinTypeDash:
        return Bitcoin::Address::isValid(string, {TWP2PKHPrefixDash, TWP2SHPrefixDash});

    case TWCoinTypeCallisto:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypePoa:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeXDai:
        return Ethereum::Address::isValid(string);

    case TWCoinTypeICON:
        return Icon::Address::isValid(string);

    case TWCoinTypeLitecoin:
        return Bitcoin::Bech32Address::isValid(string, HRP_LITECOIN) || Bitcoin::Address::isValid(string, {TWP2PKHPrefixLitecoin, TWP2SHPrefixLitecoin});

    case TWCoinTypeNimiq:
        return Nimiq::Address::isValid(string);

    case TWCoinTypeRipple:
        return Ripple::Address::isValid(string);

    case TWCoinTypeStellar:
        return Stellar::Address::isValid(string);

    case TWCoinTypeTezos:
        return Tezos::Address::isValid(string);

    case TWCoinTypeTron:
        return Tron::Address::isValid(string);

    case TWCoinTypeZcoin:
        return Bitcoin::Address::isValid(string, {TWP2PKHPrefixZcoin, TWP2SHPrefixZcoin});

    case TWCoinTypeZcash:
        return Zcash::TAddress::isValid(string, {TWP2PKHPrefixZcashT, TWP2SHPrefixZcashT});

    case TWCoinTypeNEO:
        return NEO::Address::isValid(string);
    }
}

TWPurpose TW::purpose(TWCoinType coin) {
    switch(coin) {
    case TWCoinTypeAion:
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
    case TWCoinTypeStellar:
    case TWCoinTypeTezos:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeXDai:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
    case TWCoinTypeNEO:
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
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeXDai:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        return TWCurveSECP256k1;

    case TWCoinTypeNEO:
            return TWCurveNIST256p1;

    case TWCoinTypeAion:
    case TWCoinTypeNimiq:
    case TWCoinTypeStellar:
    case TWCoinTypeTezos:
        return TWCurveEd25519;
    }
}

TWHDVersion TW::hdVersion(TWCoinType coin) {
    switch(coin) {
    case TWCoinTypeBitcoin:
    case TWCoinTypeLitecoin:
        return TWHDVersionZPUB;

    case TWCoinTypeBitcoinCash:
    case TWCoinTypeDash:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        return TWHDVersionXPUB;

    case TWCoinTypeAion:
    case TWCoinTypeBinance:
    case TWCoinTypeCallisto:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypeICON:
    case TWCoinTypeNimiq:
    case TWCoinTypePoa:
    case TWCoinTypeRipple:
    case TWCoinTypeStellar:
    case TWCoinTypeTezos:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeXDai:
    case TWCoinTypeNEO:
        return TWHDVersionNone;
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
    case TWCoinTypePoa:
    case TWCoinTypeRipple:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeXDai:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        return DerivationPath(purpose(coin), coin, 0, 0, 0);
    case TWCoinTypeAion:
    case TWCoinTypeNEO:
        return DerivationPath{
            DerivationPathIndex(purpose(coin), true),
            DerivationPathIndex(coin, true),
            DerivationPathIndex(0, true),
            DerivationPathIndex(0, true),
            DerivationPathIndex(0, true),
        };
    case TWCoinTypeNimiq:
    case TWCoinTypeTezos:
        return DerivationPath{
            DerivationPathIndex(purpose(coin), true),
            DerivationPathIndex(coin, true),
            DerivationPathIndex(0, true),
            DerivationPathIndex(0, true),
        };

    case TWCoinTypeStellar:
        return DerivationPath{
            DerivationPathIndex(purpose(coin), true),
            DerivationPathIndex(coin, true),
            DerivationPathIndex(0, true)
        };
    }
}

std::string TW::deriveAddress(TWCoinType coin, const PrivateKey& privateKey) {
    switch (coin) {
    case TWCoinTypeBinance:
        return Tendermint::Address(HRP_BINANCE_TEST, privateKey.getPublicKey(PublicKeyType::secp256k1)).string();

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
    case TWCoinTypeXDai:
        return Ethereum::Address(privateKey.getPublicKey(PublicKeyType::secp256k1Extended)).string();

    case TWCoinTypeICON:
        return Icon::Address(privateKey.getPublicKey(PublicKeyType::secp256k1Extended), TWIconAddressTypeAddress).string();

    case TWCoinTypeLitecoin:
        return Bitcoin::Bech32Address(privateKey.getPublicKey(PublicKeyType::secp256k1), 0, HRP_LITECOIN).string();

    case TWCoinTypeNimiq:
        return Nimiq::Address(privateKey.getPublicKey(PublicKeyType::ed25519)).string();

    case TWCoinTypeAion:
        return Aion::Address(privateKey.getPublicKey(PublicKeyType::ed25519)).string();

    case TWCoinTypeRipple:
        return Ripple::Address(privateKey.getPublicKey(PublicKeyType::secp256k1)).string();

    case TWCoinTypeTezos:
        return Tezos::Address(privateKey.getPublicKey(PublicKeyType::ed25519)).string();

    case TWCoinTypeTron:
        return Tron::Address(privateKey.getPublicKey(PublicKeyType::secp256k1Extended)).string();

    case TWCoinTypeZcash:
        return Zcash::TAddress(privateKey.getPublicKey(PublicKeyType::secp256k1), TWP2SHPrefixZcashT).string();

    case TWCoinTypeZcoin:
        return Bitcoin::Address(privateKey.getPublicKey(PublicKeyType::secp256k1), TWP2PKHPrefixZcoin).string();

    case TWCoinTypeStellar:
        return Stellar::Address(privateKey.getPublicKey(PublicKeyType::ed25519)).string();

    case TWCoinTypeNEO:
        return NEO::Address(privateKey.getPublicKey(PublicKeyType::nist256p1)).string();
    }
}
