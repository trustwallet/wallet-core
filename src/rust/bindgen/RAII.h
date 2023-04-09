// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "WalletCoreRSBindgen.h"

namespace TW::Rust {
    inline Data data_from_c_byte_array(CByteArrayPtr rawArray) {
        if (rawArray == nullptr) {
            return {};
        }
        if (rawArray->data == nullptr || rawArray->size == 0) {
            // We need to release the memory allocated to the `CByteArray*` pointer even if the inner `CByteArray::data` points to a null.
            free_c_byte_array(rawArray);
            return {};
        }
        Data result(&rawArray->data[0], &rawArray->data[rawArray->size]);
        free_c_byte_array(rawArray);
        return result;
    }

    inline bool data_from_c_byte_array_result(CByteArrayResult&& array_result, Data& dest) {
        if (array_result.code != OK_CODE) {
            if (array_result.result != nullptr) {
                // We need to release the memory allocated to the `CByteArray*`.
                free_c_byte_array(array_result.result);
            }
            return false;
        }
        dest = data_from_c_byte_array(array_result.result);
        return true;
    }
} // namespace TW::Rust
