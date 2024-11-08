// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{Encoded, TransactionBuilder};
use tw_coin_entry::{coin_context::CoinContext, error::prelude::*};
use tw_keypair::ed25519::sha512::PublicKey;
use tw_proto::{MessageRead, MessageWrite};

pub trait SubstrateCoinEntry {
    type SigningInput<'a>: MessageRead<'a> + MessageWrite;
    type SigningOutput: MessageWrite;
    type PreSigningOutput: MessageWrite;

    fn build_transaction(
        &self,
        coin: &dyn CoinContext,
        public_key: Option<PublicKey>,
        input: &Self::SigningInput<'_>,
    ) -> SigningResult<TransactionBuilder>;

    fn signing_output(
        &self,
        coin: &dyn CoinContext,
        result: SigningResult<Encoded>,
    ) -> Self::SigningOutput;

    fn presigning_output(
        &self,
        coin: &dyn CoinContext,
        result: SigningResult<Encoded>,
    ) -> Self::PreSigningOutput;
}
