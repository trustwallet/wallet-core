// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

namespace TW::operators::details {

template <typename T>
class empty_base {};

} // namespace TW::operators::details

namespace TW {

template <class T, class B = operators::details::empty_base<T>>
struct equality_comparable : B {
    friend bool operator!=(const T& x, const T& y) { return !(x == y); }
};

} // namespace TW
