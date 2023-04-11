// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCardano.h>

#include "Cardano/Transaction.h"
#include "Cardano/AddressV3.h"
#include "proto/Cardano.pb.h"

using namespace TW;

uint64_t TWCardanoMinAdaAmount(TWData *_Nonnull tokenBundle) {
    const Data* bundleData = static_cast<const Data*>(tokenBundle);
    TW::Cardano::Proto::TokenBundle bundleProto;
    if (bundleData && bundleProto.ParseFromArray(bundleData->data(), (int)bundleData->size())) {
        return TW::Cardano::TokenBundle::fromProto(bundleProto).minAdaAmount();
    }
    return 0;
}

TWString *_Nonnull TWCardanoGetStakingAddress(TWString *_Nonnull baseAddress) {
    const auto& address = *reinterpret_cast<const std::string*>(baseAddress);
    try {
        return TWStringCreateWithUTF8Bytes(TW::Cardano::AddressV3(address).getStakingAddress().c_str());
    } catch (...) {
        return TWStringCreateWithUTF8Bytes("");
    }
}
