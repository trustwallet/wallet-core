// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use tw_proto::{DummyMessage, MessageRead, MessageWrite, NoMessage};

pub trait WalletConnectSigner {
    type SigningInput<'a>: MessageRead<'a>;
    type SigningOutput: MessageWrite;

    /// Signs a transaction in WalletConnect format.
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput;
}

/// `NoWalletConnectSigner` can't be created since there are no enum variants.
pub enum NoWalletConnectSigner {}

impl WalletConnectSigner for NoWalletConnectSigner {
    type SigningInput<'a> = DummyMessage;
    type SigningOutput = NoMessage;

    fn sign(&self, _coin: &dyn CoinContext, _input: Self::SigningInput<'_>) -> Self::SigningOutput {
        panic!("`NoWalletConnectSigner` should never be constructed and used")
    }
}
