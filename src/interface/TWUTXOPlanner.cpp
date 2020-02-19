// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWUTXOPlanner.h>

#include "Bitcoin/Signer.h"
#include "Decred/Signer.h"
#include "Groestlcoin/Signer.h"
#include "NEO/Signer.h"
#include "Zcash/Signer.h"

#include "../proto/Bitcoin.pb.h"
#include "../proto/Decred.pb.h"
#include "../proto/NEO.pb.h"

using namespace TW;

template <typename Signer, typename Input>
TWData* _Nonnull PlanAny(TWData* _Nonnull data) {
    auto input = Input();
    input.ParseFromArray(TWDataBytes(data), (int)TWDataSize(data));
    auto serialized = Signer::plan(input).SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}

TWData* _Nonnull TWUTXOPlannerPlan(TWData* _Nonnull data, enum TWCoinType coin) {
    switch (coin) {
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
        return PlanAny<Bitcoin::Signer, Bitcoin::Proto::SigningInput>(data);
    case TWCoinTypeDecred:
        return PlanAny<Decred::Signer, Bitcoin::Proto::SigningInput>(data);
    case TWCoinTypeGroestlcoin:
        return PlanAny<Groestlcoin::Signer, Bitcoin::Proto::SigningInput>(data);
    case TWCoinTypeNEO:
        return PlanAny<NEO::Signer, NEO::Proto::SigningInput>(data);
    case TWCoinTypeZcash:
    case TWCoinTypeZelcash:
        return PlanAny<Zcash::Signer, Bitcoin::Proto::SigningInput>(data);
    default: return TWDataCreateWithSize(0);
    }
}
