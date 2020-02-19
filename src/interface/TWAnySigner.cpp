// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>

#include "Aeternity/Signer.h"
#include "Aion/Signer.h"
#include "Algorand/Signer.h"
#include "Binance/Signer.h"
#include "Bitcoin/Signer.h"
#include "Cosmos/Signer.h"
#include "Decred/Signer.h"
#include "EOS/Signer.h"
#include "Ethereum/Signer.h"
#include "FIO/Signer.h"
#include "Filecoin/Signer.h"
#include "Groestlcoin/Signer.h"
#include "Harmony/Signer.h"
#include "Icon/Signer.h"
#include "IoTeX/Signer.h"
#include "NEAR/Signer.h"
#include "NEO/Signer.h"
#include "NULS/Signer.h"
#include "Nano/Signer.h"
#include "Nebulas/Signer.h"
#include "Nimiq/Signer.h"
#include "Ontology/Signer.h"
#include "Polkadot/Signer.h"
#include "Ripple/Signer.h"
#include "Solana/Signer.h"
#include "Stellar/Signer.h"
#include "Tezos/Signer.h"
#include "Theta/Signer.h"
#include "Tron/Signer.h"
#include "VeChain/Signer.h"
#include "Wanchain/Signer.h"
#include "Waves/Signer.h"
#include "Zcash/Signer.h"
#include "Zilliqa/Signer.h"

#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

using namespace TW;

template <typename Signer, typename Input>
TWData* _Nonnull AnySign(TWData* _Nonnull data) {
    auto input = Input();
    input.ParseFromArray(TWDataBytes(data), (int)TWDataSize(data));
    auto serialized = Signer::sign(input).SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}

TWData* _Nonnull TWAnySignerSign(TWData* _Nonnull data, enum TWCoinType coin) {
    switch (coin) {
    case TWCoinTypeAeternity:
        return AnySign<Aeternity::Signer, Aeternity::Proto::SigningInput>(data);
    case TWCoinTypeAion:
        return AnySign<Aion::Signer, Aion::Proto::SigningInput>(data);
    case TWCoinTypeAlgorand:
        return AnySign<Algorand::Signer, Algorand::Proto::SigningInput>(data);
    case TWCoinTypeBinance:
        return AnySign<Binance::Signer, Binance::Proto::SigningInput>(data);
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeDash:
    case TWCoinTypeDigiByte:
    case TWCoinTypeDogecoin:
    case TWCoinTypeLitecoin:
    case TWCoinTypeMonacoin:
    case TWCoinTypeQtum:
    case TWCoinTypeRavencoin:
    case TWCoinTypeViacoin:
    case TWCoinTypeZcoin:
        return AnySign<Bitcoin::Signer, Bitcoin::Proto::SigningInput>(data);
    case TWCoinTypeDecred:
        return AnySign<Decred::Signer, Bitcoin::Proto::SigningInput>(data);
    case TWCoinTypeEOS:
        return AnySign<EOS::Signer, EOS::Proto::SigningInput>(data);
    case TWCoinTypeCosmos:
    case TWCoinTypeKava:
    case TWCoinTypeTerra:
        return AnySign<Cosmos::Signer, Cosmos::Proto::SigningInput>(data);
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeCallisto:
    case TWCoinTypeGoChain:
    case TWCoinTypePOANetwork:
    case TWCoinTypeTomoChain:
    case TWCoinTypeThunderToken:
        return AnySign<Ethereum::Signer, Ethereum::Proto::SigningInput>(data);
    case TWCoinTypeFilecoin:
        return AnySign<Filecoin::Signer, Filecoin::Proto::SigningInput>(data);
    case TWCoinTypeFIO:
        return AnySign<FIO::Signer, FIO::Proto::SigningInput>(data);
    case TWCoinTypeGroestlcoin:
        return AnySign<Groestlcoin::Signer, Bitcoin::Proto::SigningInput>(data);
    case TWCoinTypeHarmony:
        return AnySign<Harmony::Signer, Harmony::Proto::SigningInput>(data);
    case TWCoinTypeICON:
        return AnySign<Icon::Signer, Icon::Proto::SigningInput>(data);
    case TWCoinTypeIoTeX:
        return AnySign<IoTeX::Signer, IoTeX::Proto::SigningInput>(data);
    case TWCoinTypeNEAR:
        return AnySign<NEAR::Signer, NEAR::Proto::SigningInput>(data);
    case TWCoinTypeNEO:
        return AnySign<NEO::Signer, NEO::Proto::SigningInput>(data);
    case TWCoinTypeNULS:
        return AnySign<NULS::Signer, NULS::Proto::SigningInput>(data);
    case TWCoinTypeNano:
        return AnySign<Nano::Signer, Nano::Proto::SigningInput>(data);
    case TWCoinTypeNebulas:
        return AnySign<Nebulas::Signer, Nebulas::Proto::SigningInput>(data);
    case TWCoinTypeNimiq:
        return AnySign<Nimiq::Signer, Nimiq::Proto::SigningInput>(data);
    case TWCoinTypeOntology:
        return AnySign<Ontology::Signer, Ontology::Proto::SigningInput>(data);
    case TWCoinTypePolkadot:
    case TWCoinTypeKusama:
        return AnySign<Polkadot::Signer, Polkadot::Proto::SigningInput>(data);
    case TWCoinTypeXRP:
        return AnySign<Ripple::Signer, Ripple::Proto::SigningInput>(data);
    case TWCoinTypeSolana:
        return AnySign<Solana::Signer, Solana::Proto::SigningInput>(data);
    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return AnySign<Stellar::Signer, Stellar::Proto::SigningInput>(data);
    case TWCoinTypeTheta:
        return AnySign<Theta::Signer, Theta::Proto::SigningInput>(data);
    case TWCoinTypeTezos:
        return AnySign<Tezos::Signer, Tezos::Proto::SigningInput>(data);
    case TWCoinTypeTron:
        return AnySign<Tron::Signer, Tron::Proto::SigningInput>(data);
    case TWCoinTypeVeChain:
        return AnySign<VeChain::Signer, VeChain::Proto::SigningInput>(data);
    case TWCoinTypeWanchain:
        return AnySign<Wanchain::Signer, Ethereum::Proto::SigningInput>(data);
    case TWCoinTypeWaves:
        return AnySign<Waves::Signer, Waves::Proto::SigningInput>(data);
    case TWCoinTypeZilliqa:
        return AnySign<Zilliqa::Signer, Zilliqa::Proto::SigningInput>(data);
    case TWCoinTypeZcash:
    case TWCoinTypeZelcash:
        return AnySign<Zcash::Signer, Bitcoin::Proto::SigningInput>(data);
    case TWCoinTypeCardano:
    case TWCoinTypeTON:
        // not implemented yet
        break;
    }
    return TWDataCreateWithSize(0);
}

#pragma clang diagnostic pop
