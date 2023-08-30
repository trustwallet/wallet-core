// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinV2.h>

#include "rust/Wrapper.h"
#include "Data.h"

using namespace TW;

TWData* _Nonnull TWBitcoinV2Sign(enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(input));

    const Rust::TWDataWrapper dataInPtr(dataIn);
    Rust::TWDataWrapper dataOutPtr = Rust::tw_bitcoin_sign(static_cast<uint32_t>(coin), dataInPtr.get());

    auto dataOut = dataOutPtr.toDataOrDefault();
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}

TWData* _Nonnull TWBitcoinV2PreImageHashes(enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(input));

    const Rust::TWDataWrapper dataInPtr(dataIn);
    Rust::TWDataWrapper dataOutPtr = Rust::tw_bitcoin_preimage_hashes(static_cast<uint32_t>(coin), dataInPtr.get());

    auto dataOut = dataOutPtr.toDataOrDefault();
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}

TWData* _Nonnull TWBitcoinV2Compile(enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(input));

    const Rust::TWDataWrapper dataInPtr(dataIn);
    Rust::TWDataWrapper dataOutPtr = Rust::tw_bitcoin_compile(static_cast<uint32_t>(coin), dataInPtr.get());

    auto dataOut = dataOutPtr.toDataOrDefault();
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}

