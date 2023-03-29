// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <type_traits>

namespace TW {

template <typename T>
concept integral = std::is_integral_v<T>;

template <typename T>
concept floating_point = std::is_floating_point_v<T>;

} // namespace TW
