// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWUTXOPlanner.h>

#include "Coin.h"

//#include "Bitcoin/Signer.h"
//#include "Decred/Signer.h"
//#include "Groestlcoin/Signer.h"
//#include "NEO/Signer.h"
//#include "Zcash/Signer.h"

//#include "../proto/Bitcoin.pb.h"
//#include "../proto/Decred.pb.h"
//#include "../proto/NEO.pb.h"

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

    const Data& dataIn = *(reinterpret_cast<const Data*>(data));
    Data dataOut;
    bool found = TW::anyCoinPlan(coin, dataIn, dataOut);
    assert(found);
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}
