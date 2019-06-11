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
#include "Waves/Address.h"

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>
#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

using namespace TW;

bool TW::validateAddress(TWCoinType coin, const std::string& string) {
    auto& hrp = *reinterpret_cast<const std::string*>(TWCoinTypeConfigurationGetHRPPrefix(coin));
    uint8_t p2pkhPrefix = TWCoinTypeConfigurationGetP2PKHPrefix(coin);
    uint8_t p2shPrefix = TWCoinTypeConfigurationGetP2SHPrefix(coin);

    switch (coin) {
    case TWCoinTypeAion:
        return Aion::Address::isValid(string);

    case TWCoinTypeBinance:
        return Cosmos::Address::isValid(string, hrp);

    case TWCoinTypeBitcoin:
        return Bitcoin::SegwitAddress::isValid(string, hrp) ||
               Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress::isValid(string) ||
               Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeBravoCoin:
        return Bravo::Address::isValid(string);

    case TWCoinTypeCosmos:
        return Cosmos::Address::isValid(string, hrp);

    case TWCoinTypeDash:
        return Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeDecred:
        return Decred::Address::isValid(string);

    case TWCoinTypeDogecoin:
        return Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeDigiByte:
        return Bitcoin::SegwitAddress::isValid(string, hrp) ||
               Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {TWCoinTypeConfigurationGetP2SHPrefix(TWCoinTypeBitcoin)}, {p2shPrefix}});

    case TWCoinTypeGroestlcoin:
        return Bitcoin::SegwitAddress::isValid(string, hrp) ||
               Groestlcoin::Address::isValid(string, {p2pkhPrefix, p2shPrefix});

    case TWCoinTypeIocoin:
        return Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});
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
        return Bitcoin::SegwitAddress::isValid(string, hrp) ||
               Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeViacoin:
        return Bitcoin::SegwitAddress::isValid(string, hrp) ||
               Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeOntology:
        return Ontology::Address::isValid(string);

    case TWCoinTypeNimiq:
        return Nimiq::Address::isValid(string);

    case TWCoinTypeXRP:
        return Ripple::Address::isValid(string);

    case TWCoinTypeSteem:
        return Bravo::Address::isValid(string, { hrp, TW::Steem::TestnetPrefix });

    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return Stellar::Address::isValid(string);

    case TWCoinTypeTezos:
        return Tezos::Address::isValid(string);

    case TWCoinTypeTron:
        return Tron::Address::isValid(string);

    case TWCoinTypeZcoin:
        return Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeZelcash:
    case TWCoinTypeZcash:
        return Zcash::TAddress::isValid(string, {{Zcash::TAddress::staticPrefix, p2pkhPrefix}, {Zcash::TAddress::staticPrefix, p2shPrefix}});

    case TWCoinTypeZilliqa:
        return Zilliqa::isValidAddress(string);

    case TWCoinTypeNano:
        return Nano::Address::isValid(string);

    case TWCoinTypeNEO:
        return NEO::Address::isValid(string);

    case TWCoinTypeLux:
        // same p2pkh prefix as litecoin
        return Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeNULS:
        return NULS::Address::isValid(string);

    case TWCoinTypeQtum:
        return Bitcoin::SegwitAddress::isValid(string, hrp) ||
               Bitcoin::Address::isValid(string, {{p2pkhPrefix}});

    case TWCoinTypeSemux:
        return Semux::Address::isValid(string);

    case TWCoinTypeARK:
        return ARK::Address::isValid(string);
                    
    case TWCoinTypeMonetaryUnit:
        return Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeRavencoin:
        return Bitcoin::Address::isValid(string, {{p2pkhPrefix}, {p2shPrefix}});

    case TWCoinTypeWaves:
        return Waves::Address::isValid(string);
    }
}

std::string TW::deriveAddress(TWCoinType coin, const PrivateKey& privateKey) {
    auto keyType = TW::publicKeyType(coin);
    return TW::deriveAddress(coin, privateKey.getPublicKey(keyType));
}

std::string TW::deriveAddress(TWCoinType coin, const PublicKey& publicKey) {
    auto& hrp = *reinterpret_cast<const std::string*>(TWCoinTypeConfigurationGetHRPPrefix(coin));
    uint8_t p2pkhPrefix = TWCoinTypeConfigurationGetP2PKHPrefix(coin);

    switch (coin) {
    case TWCoinTypeBinance:
        return Cosmos::Address(hrp, publicKey).string();

    case TWCoinTypeBitcoin:
        return Bitcoin::SegwitAddress(publicKey, 0, hrp).string();

    case TWCoinTypeBitcoinCash:
        return Bitcoin::CashAddress(publicKey).string();

    case TWCoinTypeBravoCoin:
        return Bravo::Address(publicKey).string();

    case TWCoinTypeCosmos:
        return Cosmos::Address(hrp, publicKey).string();

    case TWCoinTypeDash:
        return Bitcoin::Address(publicKey, p2pkhPrefix).string();

    case TWCoinTypeDecred:
        return Decred::Address(publicKey).string();

    case TWCoinTypeDogecoin:
        return Bitcoin::Address(publicKey, p2pkhPrefix).string();

    case TWCoinTypeDigiByte:
        return Bitcoin::SegwitAddress(publicKey, 0, hrp).string();

    case TWCoinTypeGroestlcoin:
        return Bitcoin::SegwitAddress(publicKey, 0, hrp).string();

    case TWCoinTypeIocoin:
        return Iocoin::Address(publicKey, p2pkhPrefix).string();

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
        return Bitcoin::SegwitAddress(publicKey, 0, hrp).string();
    case TWCoinTypeViacoin:
        return Bitcoin::SegwitAddress(publicKey, 0, hrp).string();
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
        return Zcash::TAddress(publicKey, p2pkhPrefix).string();

    case TWCoinTypeZcoin:
        return Bitcoin::Address(publicKey, p2pkhPrefix).string();

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
        return Bitcoin::Address(publicKey, p2pkhPrefix).string();

    case TWCoinTypeNULS:
        return NULS::Address(publicKey).string();

    case TWCoinTypeQtum:
        return Bitcoin::Address(publicKey, p2pkhPrefix).string();

    case TWCoinTypeSemux:
        return Semux::Address(publicKey).string();

    case TWCoinTypeARK:
        return ARK::Address(publicKey).string();
        
    case TWCoinTypeMonetaryUnit:
        return Bitcoin::Address(publicKey, p2pkhPrefix).string();

    case TWCoinTypeRavencoin:
        return Bitcoin::Address(publicKey, p2pkhPrefix).string();

    case TWCoinTypeWaves:
        return Waves::Address(publicKey).string();
    }
}

#pragma clang diagnostic pop
