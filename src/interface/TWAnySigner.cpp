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
#include "Bitcoin/TransactionSigner.h"
#include "Cosmos/Signer.h"
#include "Decred/Signer.h"
#include "EOS/Signer.h"
#include "Ethereum/Signer.h"
#include "Filecoin/Signer.h"
#include "FIO/Signer.h"
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
#include "Theta/Signer.h"
#include "Tezos/Signer.h"
#include "Tron/Signer.h"
#include "VeChain/Signer.h"
#include "Wanchain/Signer.h"
#include "Waves/Signer.h"
#include "Zilliqa/Signer.h"

using namespace TW;

template <typename Signer, typename Input>
TWData* _Nonnull SimpleSign(TWData* _Nonnull data) {
    auto input = Input();
    input.ParseFromArray(TWDataBytes(data), (int)TWDataSize(data));
    auto serialized = Signer::sign(input).SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}

TWData* _Nonnull TWAnySignerSign(TWData* _Nonnull data, enum TWCoinType coin) {
    switch (coin) {
    case TWCoinTypeAeternity:
        return SimpleSign<Aeternity::Signer, Aeternity::Proto::SigningInput>(data);
    case TWCoinTypeAion:
        return SimpleSign<Aion::Signer, Aion::Proto::SigningInput>(data);
    case TWCoinTypeAlgorand:
        return SimpleSign<Algorand::Signer, Algorand::Proto::SigningInput>(data);
    case TWCoinTypeBinance:
        return SimpleSign<Binance::Signer, Binance::Proto::SigningInput>(data);
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeLitecoin:
    case TWCoinTypeDogecoin:
    case TWCoinTypeDash:
    case TWCoinTypeZcoin:
    case TWCoinTypeQtum:
    case TWCoinTypeViacoin:
    case TWCoinTypeRavencoin:
    case TWCoinTypeDigiByte:
        // FIXME
        break;
    case TWCoinTypeDecred:
        // FIXME
        break;
    case TWCoinTypeEOS:
        return SimpleSign<EOS::Signer, EOS::Proto::SigningInput>(data);
    case TWCoinTypeCosmos:
    case TWCoinTypeKava:
    case TWCoinTypeTerra:
        return SimpleSign<Cosmos::Signer, Cosmos::Proto::SigningInput>(data);
    case TWCoinTypeEthereum:
        return SimpleSign<Ethereum::Signer, Ethereum::Proto::SigningInput>(data);
    case TWCoinTypeFilecoin:
        return SimpleSign<Filecoin::Signer, Filecoin::Proto::SigningInput>(data);
    case TWCoinTypeFIO:
        return SimpleSign<FIO::Signer, FIO::Proto::SigningInput>(data);
    case TWCoinTypeGroestlcoin:
        //FIXME
        break;
    case TWCoinTypeHarmony:
        return SimpleSign<Harmony::Signer, Harmony::Proto::SigningInput>(data);
    case TWCoinTypeICON:
        return SimpleSign<Icon::Signer, Icon::Proto::SigningInput>(data);
    case TWCoinTypeIoTeX:
        return SimpleSign<IoTeX::Signer, IoTeX::Proto::SigningInput>(data);
    case TWCoinTypeNEAR:
        return SimpleSign<NEAR::Signer, NEAR::Proto::SigningInput>(data);
    case TWCoinTypeNEO:
        // FIXME
        break;
    case TWCoinTypeNULS:
        return SimpleSign<NULS::Signer, NULS::Proto::SigningInput>(data);
    case TWCoinTypeNano:
        return SimpleSign<Nano::Signer, Nano::Proto::SigningInput>(data);
    case TWCoinTypeNebulas:
        return SimpleSign<Nebulas::Signer, Nebulas::Proto::SigningInput>(data);
    case TWCoinTypeNimiq:
        return SimpleSign<Nimiq::Signer, Nimiq::Proto::SigningInput>(data);
    case TWCoinTypeOntology:
        return SimpleSign<Ontology::Signer, Ontology::Proto::SigningInput>(data);
    case TWCoinTypePolkadot:
    case TWCoinTypeKusama:
        return SimpleSign<Polkadot::Signer, Polkadot::Proto::SigningInput>(data);
    case TWCoinTypeXRP:
        return SimpleSign<Ripple::Signer, Ripple::Proto::SigningInput>(data);
    case TWCoinTypeSolana:
        return SimpleSign<Solana::Signer, Solana::Proto::SigningInput>(data);
    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return SimpleSign<Stellar::Signer, Stellar::Proto::SigningInput>(data);
    case TWCoinTypeTheta:
        return SimpleSign<Theta::Signer, Theta::Proto::SigningInput>(data);
    case TWCoinTypeTezos:
        return SimpleSign<Tezos::Signer, Tezos::Proto::SigningInput>(data);
    case TWCoinTypeTron:
        return SimpleSign<Tron::Signer, Tron::Proto::SigningInput>(data);
    case TWCoinTypeVeChain:
        return SimpleSign<VeChain::Signer, VeChain::Proto::SigningInput>(data);
    case TWCoinTypeWanchain:
        return SimpleSign<Wanchain::Signer, Ethereum::Proto::SigningInput>(data);
    case TWCoinTypeWaves:
        return SimpleSign<Waves::Signer, Waves::Proto::SigningInput>(data);
    case TWCoinTypeZilliqa:
        return SimpleSign<Zilliqa::Signer, Zilliqa::Proto::SigningInput>(data);
    default: break;
    }
    return TWDataCreateWithSize(0);
}

TWData* _Nonnull TWAnySignerSignWithPlan(TWData* _Nonnull data, TWData* _Nonnull plan, enum TWCoinType coin) {
    return TWDataCreateWithSize(0);
}
