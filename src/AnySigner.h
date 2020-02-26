// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWData.h>

namespace TW {

template <typename Signer, typename Input>
TWData* _Nonnull AnySign(TWData* _Nonnull data) {
    auto input = Input();
    input.ParseFromArray(TWDataBytes(data), (int)TWDataSize(data));
    auto serialized = Signer::sign(input).SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}

// TODO: Is this template really needed?  Each typed Entry.cpp can include it's own typed version anyways
// Note: use output parameter to avoid unneeded copies
template <typename Signer, typename Input>
void AnySignTempl(const Data& dataIn, Data& dataOut) {
    auto input = Input();
    input.ParseFromArray(dataIn.data(), (int)dataIn.size());
    auto serialized = Signer::sign(input).SerializeAsString();
    dataOut.insert(dataOut.end(), serialized.begin(), serialized.end());
}

} // namespace TW
