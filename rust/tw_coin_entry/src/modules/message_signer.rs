// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_context::CoinContext;
use tw_proto::{DummyMessage, MessageRead, MessageWrite, NoMessage};

pub trait MessageSigner {
    type MessageSigningInput<'a>: MessageRead<'a>;
    type MessagePreSigningOutput: MessageWrite;
    type MessageSigningOutput: MessageWrite;
    type MessageVerifyingInput<'a>: MessageRead<'a>;

    /// Returns hash(es) for signing, needed for external signing.
    fn message_preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessagePreSigningOutput;

    /// Signs a message.
    fn sign_message(
        &self,
        coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessageSigningOutput;

    /// Verifies signature.
    fn verify_message(
        &self,
        _coin: &dyn CoinContext,
        input: Self::MessageVerifyingInput<'_>,
    ) -> bool;
}

/// `NoMessageSigner` can't be created since there are no enum variants.
pub enum NoMessageSigner {}

impl MessageSigner for NoMessageSigner {
    type MessageSigningInput<'a> = DummyMessage;
    type MessagePreSigningOutput = NoMessage;
    type MessageSigningOutput = NoMessage;
    type MessageVerifyingInput<'a> = DummyMessage;

    fn message_preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        _input: Self::MessageSigningInput<'_>,
    ) -> Self::MessagePreSigningOutput {
        panic!("`NoMessageSigner` should never be constructed and used")
    }

    fn sign_message(
        &self,
        _coin: &dyn CoinContext,
        _input: Self::MessageSigningInput<'_>,
    ) -> Self::MessageSigningOutput {
        panic!("`NoMessageSigner` should never be constructed and used")
    }

    fn verify_message(
        &self,
        _coin: &dyn CoinContext,
        _input: Self::MessageVerifyingInput<'_>,
    ) -> bool {
        panic!("`NoMessageSigner` should never be constructed and used")
    }
}
