// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ed25519;
use tw_keypair::traits::KeyPairTrait;
use tw_proto::Pactus::Proto;

use crate::transaction::Transaction;

pub struct PactusSigner;

impl PactusSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let mut trx = Transaction::from_proto(&input)?;
        let key_pair = ed25519::sha512::KeyPair::try_from(input.private_key.as_ref())?;
        trx.sign(key_pair.private())?;

        let data = trx.to_bytes()?;

        let output = Proto::SigningOutput {
            encoded: data.into(),
            ..Proto::SigningOutput::default()
        };

        Ok(output)
    }
}
