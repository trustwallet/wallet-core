// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::de::DeserializeOwned;
use serde::Serialize;

pub mod common_fields;
pub mod json_transaction;
pub mod transaction_builder;
pub mod transaction_type;
pub mod transactions;

#[macro_export]
macro_rules! ripple_tx {
    ($tx:ty) => {
        impl $crate::transaction::RippleTransaction for $tx {
            fn common_types(&self) -> &$crate::transaction::common_fields::CommonFields {
                &self.common_fields
            }

            fn common_types_mut(
                &mut self,
            ) -> &mut $crate::transaction::common_fields::CommonFields {
                &mut self.common_fields
            }
        }
    };
}

pub trait RippleTransaction: DeserializeOwned + Serialize {
    fn common_types(&self) -> &common_fields::CommonFields;

    fn common_types_mut(&mut self) -> &mut common_fields::CommonFields;
}
