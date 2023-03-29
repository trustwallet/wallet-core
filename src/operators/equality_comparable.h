// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
