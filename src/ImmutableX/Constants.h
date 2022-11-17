// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "uint256.h"

namespace TW::ImmutableX {

namespace internal {
inline constexpr const char* gLayer = "starkex";
inline constexpr const char* gApplication = "immutablex";
inline constexpr const char* gIndex = "1";
inline const uint256_t gStarkCurveN("3618502788666131213697322783095070105526743751716087489154079457884512865583");
inline const uint256_t gStarkDeriveBias("112173586448650067624617006275947173271329056303198712163776463194419898833073");
} // namespace internal

}
