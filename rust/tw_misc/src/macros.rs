// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#[macro_export]
macro_rules! try_or_false {
    ($exp:expr) => {{
        match $crate::traits::IntoOption::into_option($exp) {
            Some(res) => res,
            None => return false,
        }
    }};
}

#[macro_export]
macro_rules! try_or_else {
    ($exp:expr, $or_else:expr) => {{
        match $crate::traits::IntoOption::into_option($exp) {
            Some(res) => res,
            None => return $or_else(),
        }
    }};
}
