// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAvalancheDecoder.h>

#include "Data.h"
#include "HexCoding.h"

#include "Avalanche/Decoder.h"

#include <cassert>
#include <string>
#include <vector>

using namespace TW;

/// Wrapper for C interface, empty as all methods are static
struct TWAvalancheDecoder {};

TWString* _Nullable TWAvalancheDecoderDecodeUTXO(TWData* _Nonnull utxoData) {
    const Data& data = *(reinterpret_cast<const Data*>(utxoData));
    try {     
        auto string = Avalanche::decodeUTXO(data);
        if (!string.has_value()) {
            return nullptr;
        }
        return TWStringCreateWithUTF8Bytes(string->c_str());
    }
    catch(...) {
        return nullptr;
    }
}
