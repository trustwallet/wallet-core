// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"

#include "Aeternity/Address.h"
#include "Aion/Address.h"
#include "Algorand/Address.h"
#include "Binance/Address.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bravo/Address.h"
#include "Cosmos/Address.h"
#include "Decred/Address.h"
#include "EOS/Address.h"
#include "Ethereum/Address.h"
#include "FIO/Address.h"
#include "Groestlcoin/Address.h"
#include "Harmony/Address.h"
#include "Icon/Address.h"
#include "IoTeX/Address.h"
#include "NEAR/Address.h"
#include "Nano/Address.h"
#include "Nebulas/Address.h"
#include "Nimiq/Address.h"
#include "NULS/Address.h"
#include "Ontology/Address.h"
#include "Ripple/Address.h"
#include "Solana/Address.h"
#include "Stellar/Address.h"
#include "Tezos/Address.h"
#include "Tron/Address.h"
#include "Wanchain/Address.h"
#include "Waves/Address.h"
#include "Zcash/TAddress.h"
#include "Zilliqa/Address.h"

#include <TrustWalletCore/TWHRP.h>

#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

using namespace TW;

bool TW::validateAddress(TWCoinType coin, const std::string &string) {
    auto p2pkh = TW::p2pkhPrefix(coin);
    auto p2sh = TW::p2shPrefix(coin);
    auto hrp = stringForHRP(TW::hrp(coin));
    switch (coin) {
    case TWCoinTypeAeternity:
        return Aeternity::Address::isValid(string);

    case TWCoinTypeAion:
        return Aion::Address::isValid(string);

    case TWCoinTypeBinance:
        return Binance::Address::isValid(string);

    case TWCoinTypeCosmos:
    case TWCoinTypeTerra:
        return Cosmos::Address::isValid(string, hrp);

    case TWCoinTypeBitcoin:
    case TWCoinTypeDigiByte:
    case TWCoinTypeLitecoin:
    case TWCoinTypeMonacoin:
    case TWCoinTypeQtum:
    case TWCoinTypeViacoin:
        return Bitcoin::SegwitAddress::isValid(string, hrp) ||
               Bitcoin::Address::isValid(string, {{p2pkh}, {p2sh}});

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress::isValid(string) ||
               Bitcoin::Address::isValid(string, {{p2pkh}, {p2sh}});

    case TWCoinTypeBravoCoin:
        return Bravo::Address::isValid(string);

    case TWCoinTypeDash:
    case TWCoinTypeDogecoin:
    case TWCoinTypeRavencoin:
    case TWCoinTypeZcoin:
        return Bitcoin::Address::isValid(string, {{p2pkh}, {p2sh}});

    case TWCoinTypeDecred:
        return Decred::Address::isValid(string);

    case TWCoinTypeGroestlcoin:
        return Bitcoin::SegwitAddress::isValid(string, hrp) ||
               Groestlcoin::Address::isValid(string, {p2pkh, p2sh});

    case TWCoinTypeCallisto:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGoChain:
    case TWCoinTypePOANetwork:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeTheta:
        return Ethereum::Address::isValid(string);

    case TWCoinTypeEOS:
        return EOS::Address::isValid(string);
    case TWCoinTypeFIO:
        return FIO::Address::isValid(string);

    case TWCoinTypeWanchain:
        return Wanchain::Address::isValid(string);

    case TWCoinTypeICON:
        return Icon::Address::isValid(string);

    case TWCoinTypeIoTeX:
        return IoTeX::Address::isValid(string);

    case TWCoinTypeOntology:
        return Ontology::Address::isValid(string);

    case TWCoinTypeNimiq:
        return Nimiq::Address::isValid(string);

    case TWCoinTypeXRP:
        return Ripple::Address::isValid(string);

    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return Stellar::Address::isValid(string);

    case TWCoinTypeTezos:
        return Tezos::Address::isValid(string);

    case TWCoinTypeTron:
        return Tron::Address::isValid(string);

    case TWCoinTypeZelcash:
    case TWCoinTypeZcash:
        return Zcash::TAddress::isValid(string, {{Zcash::TAddress::staticPrefix, p2pkh},
                                                 {Zcash::TAddress::staticPrefix, p2sh}});

    case TWCoinTypeZilliqa:
        return Zilliqa::Address::isValid(string);

    case TWCoinTypeNano:
        return Nano::Address::isValid(string);

    case TWCoinTypeNEAR:
        return NEAR::Address::isValid(string);

    case TWCoinTypeNULS:
        return NULS::Address::isValid(string);

    case TWCoinTypeWaves:
        return Waves::Address::isValid(string);

    case TWCoinTypeNebulas:
        return Nebulas::Address::isValid(string);

    case TWCoinTypeHarmony:
        return Harmony::Address::isValid(string);

    case TWCoinTypeSolana:
        return Solana::Address::isValid(string);

    case TWCoinTypeAlgorand:
        return Algorand::Address::isValid(string);
    }
}

std::string TW::deriveAddress(TWCoinType coin, const PrivateKey &privateKey) {
    auto keyType = TW::publicKeyType(coin);
    return TW::deriveAddress(coin, privateKey.getPublicKey(keyType));
}

std::string TW::deriveAddress(TWCoinType coin, const PublicKey &publicKey) {
    auto p2pkh = TW::p2pkhPrefix(coin);
    auto hrp = stringForHRP(TW::hrp(coin));

    switch (coin) {
    case TWCoinTypeAeternity:
        return Aeternity::Address(publicKey).string();

    case TWCoinTypeBinance:
        return Binance::Address(publicKey).string();

    case TWCoinTypeBitcoin:
    case TWCoinTypeDigiByte:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeLitecoin:
    case TWCoinTypeViacoin:
        return Bitcoin::SegwitAddress(publicKey, 0, hrp).string();

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress(publicKey).string();

    case TWCoinTypeBravoCoin:
        return Bravo::Address(publicKey).string();

    case TWCoinTypeCosmos:
    case TWCoinTypeTerra:
        return Cosmos::Address(hrp, publicKey).string();

    case TWCoinTypeDash:
    case TWCoinTypeDogecoin:
    case TWCoinTypeMonacoin:
    case TWCoinTypeQtum:
    case TWCoinTypeRavencoin:
    case TWCoinTypeZcoin:
        return Bitcoin::Address(publicKey, p2pkh).string();

    case TWCoinTypeDecred:
        return Decred::Address(publicKey).string();

    case TWCoinTypeCallisto:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGoChain:
    case TWCoinTypePOANetwork:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeTheta:
        return Ethereum::Address(publicKey).string();

    case TWCoinTypeEOS:
        return EOS::Address(publicKey).string();

    case TWCoinTypeFIO:
        return FIO::Address(publicKey).string();

    case TWCoinTypeWanchain:
        return Wanchain::Address(publicKey).string();

    case TWCoinTypeICON:
        return Icon::Address(publicKey, TWIconAddressTypeAddress).string();

    case TWCoinTypeIoTeX:
        return IoTeX::Address(publicKey).string();

    case TWCoinTypeOntology:
        return Ontology::Address(publicKey).string();

    case TWCoinTypeNimiq:
        return Nimiq::Address(publicKey).string();

    case TWCoinTypeAion:
        return Aion::Address(publicKey).string();

    case TWCoinTypeXRP:
        return Ripple::Address(publicKey).string();

    case TWCoinTypeTezos:
        return Tezos::Address(publicKey).string();

    case TWCoinTypeTron:
        return Tron::Address(publicKey).string();

    case TWCoinTypeZelcash:
    case TWCoinTypeZcash:
        return Zcash::TAddress(publicKey, p2pkh).string();

    case TWCoinTypeZilliqa:
        return Zilliqa::Address(publicKey).string();

    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return Stellar::Address(publicKey).string();

    case TWCoinTypeNano:
        return Nano::Address(publicKey).string();

    case TWCoinTypeNULS:
        return NULS::Address(publicKey).string();
        
    case TWCoinTypeNEAR:
        return NEAR::Address(publicKey).string();

    case TWCoinTypeWaves:
        return Waves::Address(publicKey).string();

    case TWCoinTypeNebulas:
        return Nebulas::Address(publicKey).string();

    case TWCoinTypeHarmony:
        return Harmony::Address(publicKey).string();

    case TWCoinTypeSolana:
        return Solana::Address(publicKey).string();

    case TWCoinTypeAlgorand:
        return Algorand::Address(publicKey).string();
    }
}

#pragma clang diagnostic pop
