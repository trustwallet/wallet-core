// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCardano.h>

#include "Cardano/Transaction.h"
#include "proto/Cardano.pb.h"

using namespace TW::Cardano;
using namespace TW;

uint64_t TWCardanoMinAdaAmount(TWData *_Nonnull tokenBundle) {
    const Data* bundleData = static_cast<const Data*>(tokenBundle);
    Proto::TokenBundle bundleProto;
    if (bundleData && bundleProto.ParseFromArray(bundleData->data(), (int)bundleData->size())) {
        return TokenBundle::fromProto(bundleProto).minAdaAmount();
    }
    return 0;
}
