// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"

#include "Aion/Address.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/CashAddress.h"
#include "Decred/Address.h"
#include "Ethereum/Address.h"
#include "Groestlcoin/Address.h"
#include "IOST/Account.h"
#include "Icon/Address.h"
#include "Iocoin/Address.h"
#include "Nano/Address.h"
#include "NEO/Address.h"
#include "Nimiq/Address.h"
#include "Ontology/Address.h"
#include "Ripple/Address.h"
#include "Stellar/Address.h"
#include "Cosmos/Address.h"
#include "Tezos/Address.h"
#include "Tron/Address.h"
#include "Wanchain/Address.h"
#include "Zcash/TAddress.h"
#include "NULS/Address.h"
#include "Bravo/Address.h"
#include "Steem/Address.h"
#include "EOS/Address.h"
#include "IoTeX/Address.h"
#include "Zilliqa/Address.h"
#include "Semux/Address.h"
#include "ARK/Address.h"

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>

#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

using namespace TW;

bool TW::validateAddress(TWCoinType coin, const std::string& string) {
    switch (coin) {
    case TWCoinTypeAion:
        return Aion::Address::isValid(string);

    case TWCoinTypeBinance:
        return Cosmos::Address::isValid(string, HRP_BINANCE);

    case TWCoinTypeBitcoin:
        return Bitcoin::SegwitAddress::isValid(string, HRP_BITCOIN) ||
               Bitcoin::Address::isValid(string, {{TWP2PKHPrefixBitcoin}, {TWP2SHPrefixBitcoin}});

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress::isValid(string) ||
               Bitcoin::Address::isValid(string, {{TWP2PKHPrefixBitcoin}, {TWP2SHPrefixBitcoin}});

    case TWCoinTypeBravoCoin:
        return Bravo::Address::isValid(string);

    case TWCoinTypeCosmos:
        return Cosmos::Address::isValid(string, HRP_COSMOS);

    case TWCoinTypeDash:
        return Bitcoin::Address::isValid(string, {{TWP2PKHPrefixDash}, {TWP2SHPrefixDash}});

    case TWCoinTypeDecred:
        return Decred::Address::isValid(string);

    case TWCoinTypeDogecoin:
        return Bitcoin::Address::isValid(string, {{TWP2PKHPrefixDogecoin}, {TWP2SHPrefixDogecoin}});

    case TWCoinTypeGroestlcoin:
        return Bitcoin::SegwitAddress::isValid(string, HRP_GROESTLCOIN) ||
               Groestlcoin::Address::isValid(string, {TWP2PKHPrefixGroestlcoin, TWP2SHPrefixGroestlcoin});

    case TWCoinTypeIocoin:
        return Bitcoin::Address::isValid(string, {{TWP2PKHPrefixIocoin}, {TWP2SHPrefixIocoin}});
    case TWCoinTypeCallisto:
    case TWCoinTypeEllaism:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeEthersocial:
    case TWCoinTypeGoChain:
    case TWCoinTypePOANetwork:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeXDai:
    case TWCoinTypeTheta:
    case TWCoinTypeDEXON:
        return Ethereum::Address::isValid(string);

    case TWCoinTypeEOS:
        return EOS::Address::isValid(string);

    case TWCoinTypeWanchain:
        return Wanchain::Address::isValid(string);
    case TWCoinTypeICON:
        return Icon::Address::isValid(string);
    case TWCoinTypeIOST:
        return IOST::Account::isValid(string);
    case TWCoinTypeIoTeX:
        return IoTeX::Address::isValid(string);
    case TWCoinTypeLitecoin:
        return Bitcoin::SegwitAddress::isValid(string, HRP_LITECOIN) ||
               Bitcoin::Address::isValid(string, {{TWP2PKHPrefixLitecoin}, {TWP2SHPrefixLitecoin}});

    case TWCoinTypeViacoin:
        return Bitcoin::SegwitAddress::isValid(string, HRP_VIACOIN) ||
               Bitcoin::Address::isValid(string, {{TWP2PKHPrefixViacoin}, {TWP2SHPrefixViacoin}});

    case TWCoinTypeOntology:
        return Ontology::Address::isValid(string);

    case TWCoinTypeNimiq:
        return Nimiq::Address::isValid(string);

    case TWCoinTypeXRP:
        return Ripple::Address::isValid(string);

    case TWCoinTypeSteem:
        return Bravo::Address::isValid(string, { TW::Steem::MainnetPrefix, TW::Steem::TestnetPrefix });

    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return Stellar::Address::isValid(string);

    case TWCoinTypeTezos:
        return Tezos::Address::isValid(string);

    case TWCoinTypeTron:
        return Tron::Address::isValid(string);

    case TWCoinTypeZcoin:
        return Bitcoin::Address::isValid(string, {{TWP2PKHPrefixZcoin}, {TWP2SHPrefixZcoin}});

    case TWCoinTypeZelcash:
    case TWCoinTypeZcash:
        return Zcash::TAddress::isValid(string, {{Zcash::TAddress::staticPrefix, TWP2PKHPrefixZcashT}, {Zcash::TAddress::staticPrefix, TWP2SHPrefixZcashT}});

    case TWCoinTypeZilliqa:
        return Zilliqa::Address::isValid(string);

    case TWCoinTypeNano:
        return Nano::Address::isValid(string);

    case TWCoinTypeNEO:
        return NEO::Address::isValid(string);

    case TWCoinTypeLux:
        // same p2pkh prefix as litecoin
        return Bitcoin::Address::isValid(string, {{TWP2PKHPrefixLitecoin}, {TWP2SHPrefixLux}});

    case TWCoinTypeNULS:
        return NULS::Address::isValid(string);

    case TWCoinTypeQtum:
        return Bitcoin::SegwitAddress::isValid(string, HRP_QTUM) ||
               Bitcoin::Address::isValid(string, {{TWP2PKHPrefixQtum}});

    case TWCoinTypeSemux:
        return Semux::Address::isValid(string);

    case TWCoinTypeARK:
        return ARK::Address::isValid(string);
    }
}

std::string TW::deriveAddress(TWCoinType coin, const PrivateKey& privateKey) {
    auto keyType = TW::publicKeyType(coin);
    return TW::deriveAddress(coin, privateKey.getPublicKey(keyType));
}

std::string TW::deriveAddress(TWCoinType coin, const PublicKey& publicKey) {
    switch (coin) {
    case TWCoinTypeBinance:
        return Cosmos::Address(HRP_BINANCE, publicKey).string();

    case TWCoinTypeCosmos:
        return Cosmos::Address(HRP_COSMOS, publicKey).string();

    case TWCoinTypeBitcoin:
        return Bitcoin::SegwitAddress(publicKey, 0, HRP_BITCOIN).string();

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress(publicKey).string();

    case TWCoinTypeBravoCoin:
        return Bravo::Address(publicKey).string();

    case TWCoinTypeDash:
        return Bitcoin::Address(publicKey, TWP2PKHPrefixDash).string();

    case TWCoinTypeDecred:
        return Decred::Address(publicKey).string();

    case TWCoinTypeDogecoin:
        return Bitcoin::Address(publicKey, TWP2PKHPrefixDogecoin).string();

    case TWCoinTypeGroestlcoin:
        return Bitcoin::SegwitAddress(publicKey, 0, HRP_GROESTLCOIN).string();

    case TWCoinTypeIocoin:
        return Iocoin::Address(publicKey, TWP2PKHPrefixIocoin).string();

    case TWCoinTypeCallisto:
    case TWCoinTypeEllaism:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeEthersocial:
    case TWCoinTypeGoChain:
    case TWCoinTypePOANetwork:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeXDai:
    case TWCoinTypeTheta:
    case TWCoinTypeDEXON:
        return Ethereum::Address(publicKey).string();

    case TWCoinTypeEOS:
        return EOS::Address(publicKey).string();

    case TWCoinTypeWanchain:
        return Wanchain::Address(publicKey).string();
    case TWCoinTypeICON:
        return Icon::Address(publicKey, TWIconAddressTypeAddress).string();
    case TWCoinTypeIOST:
        return IOST::Account::encodePubKey(publicKey);
    case TWCoinTypeIoTeX:
        return IoTeX::Address(publicKey).string();
    case TWCoinTypeLitecoin:
        return Bitcoin::SegwitAddress(publicKey, 0, HRP_LITECOIN).string();
    case TWCoinTypeViacoin:
        return Bitcoin::SegwitAddress(publicKey, 0, HRP_VIACOIN).string();
    case TWCoinTypeOntology:
        return Ontology::Address(publicKey).string();
    case TWCoinTypeNimiq:
        return Nimiq::Address(publicKey).string();

    case TWCoinTypeAion:
        return Aion::Address(publicKey).string();

    case TWCoinTypeXRP:
        return Ripple::Address(publicKey).string();

    case TWCoinTypeSteem:
        return Bravo::Address(publicKey, TW::Steem::MainnetPrefix).string();

    case TWCoinTypeTezos:
        return Tezos::Address(publicKey).string();

    case TWCoinTypeTron:
        return Tron::Address(publicKey).string();

    case TWCoinTypeZelcash:
    case TWCoinTypeZcash:
        return Zcash::TAddress(publicKey, TWP2PKHPrefixZcashT).string();

    case TWCoinTypeZcoin:
        return Bitcoin::Address(publicKey, TWP2PKHPrefixZcoin).string();

    case TWCoinTypeZilliqa:
        return Zilliqa::Address(publicKey).string();

    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return Stellar::Address(publicKey).string();

    case TWCoinTypeNano:
        return Nano::Address(publicKey).string();

    case TWCoinTypeNEO:
        return NEO::Address(publicKey).string();
    case TWCoinTypeLux:
        return Bitcoin::Address(publicKey, TWP2PKHPrefixLitecoin).string();

    case TWCoinTypeNULS:
        return NULS::Address(publicKey).string();

    case TWCoinTypeQtum:
        return Bitcoin::Address(publicKey, TWP2PKHPrefixQtum).string();

    case TWCoinTypeSemux:
        return Semux::Address(publicKey).string();

    case TWCoinTypeARK:
        return ARK::Address(publicKey).string();
    }
}

#pragma clang diagnostic pop
