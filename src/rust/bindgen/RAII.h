// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "rust/bindgen/WalletCoreRSBindgen.h"

namespace TW::Rust {
    Data data_from_c_byte_array(CByteArray&& rawArray) {
        Data result(&rawArray.data[0], &rawArray.data[rawArray.size]);
        std::free(rawArray.data);
        return result;
    }
}
