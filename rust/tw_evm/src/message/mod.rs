// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_hash::H256;

pub mod eip191_message;
pub mod signature;

pub type EthMessageBoxed = Box<dyn EthMessage>;

pub trait EthMessage {
    fn into_boxed(self) -> EthMessageBoxed
    where
        Self: 'static + Sized,
    {
        Box::new(self)
    }

    /// Returns message data.
    fn data(&self) -> Vec<u8>;

    /// Returns hash of the message.
    fn hash(&self) -> H256;
}
