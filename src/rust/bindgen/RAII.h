// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "iostream"
#include "rust/bindgen/WalletCoreRSBindgen.h"

namespace TW::Rust {
    inline Data data_from_c_byte_array(CByteArray&& rawArray) {
        if (rawArray.data == nullptr || rawArray.size == 0) {
            return {};
        }
        Data result(&rawArray.data[0], &rawArray.data[rawArray.size]);
        std::free(rawArray.data);
        return result;
    }

    inline bool data_from_c_byte_array_result(CByteArrayResult&& result, Data& dest) {
        if (result.code != OK_CODE) {
            return false;
        }
        dest = data_from_c_byte_array(std::move(result.result));
        return true;
    }
} // namespace TW::Rust
