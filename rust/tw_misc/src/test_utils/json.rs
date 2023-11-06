// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde_json::Value as Json;

pub trait ToJson {
    fn to_json(&self) -> Json;
}

impl<T> ToJson for T
where
    T: AsRef<str>,
{
    #[track_caller]
    fn to_json(&self) -> Json {
        serde_json::from_str(self.as_ref()).expect("Error on deserializing JSON from string")
    }
}

#[macro_export]
macro_rules! assert_json {
    ($left:expr, $right:expr) => {{
        use $crate::test_utils::json::ToJson;

        let left = $left.to_json();
        let right = $right.to_json();
        assert_eq!(left, right);
    }};
}
