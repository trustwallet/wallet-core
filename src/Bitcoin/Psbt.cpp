// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Psbt.h"
#include "rust/Wrapper.h"

namespace TW::Bitcoin {

Data Psbt::sign(const Data &input, TWCoinType coin) {
    const Rust::TWDataWrapper inputRust = input;
    const Rust::TWDataWrapper outputRust = Rust::tw_bitcoin_psbt_sign(inputRust.get(), static_cast<uint32_t>(coin));
    return outputRust.toDataOrDefault();
}

Data Psbt::plan(const Data& input, TWCoinType coin) {
    const Rust::TWDataWrapper inputRust = input;
    const Rust::TWDataWrapper outputRust = Rust::tw_bitcoin_psbt_plan(inputRust.get(), static_cast<uint32_t>(coin));
    return outputRust.toDataOrDefault();
}

} // namespace TW::Bitcoin
