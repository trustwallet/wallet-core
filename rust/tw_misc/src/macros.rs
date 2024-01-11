// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
