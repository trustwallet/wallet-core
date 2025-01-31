// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::de::DeserializeOwned;
use serde::{Deserialize, Serialize};

pub mod common_fields;
pub mod transaction_type;
pub mod transactions;

#[macro_export]
macro_rules! ripple_tx {
    ($tx:ty) => {
        impl $crate::transaction::RippleTransaction for $tx {
            fn common_types_mut(
                &mut self,
            ) -> &mut $crate::transaction::common_fields::CommonFields {
                &mut self.common_fields
            }
        }
    };
}

pub trait RippleTransaction: DeserializeOwned + Serialize {
    fn common_types_mut(&mut self) -> &mut common_fields::CommonFields;
}
