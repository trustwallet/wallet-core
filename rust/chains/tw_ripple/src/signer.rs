// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encode_tx;
use crate::modules::protobuf_builder::ProtobufBuilder;
use crate::modules::transaction_signer::TransactionSigner;
use crate::transaction::RippleTransaction;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ecdsa::secp256k1;
use tw_proto::Ripple::Proto;

pub struct RippleSigner;

impl RippleSigner {
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
        if input.raw_json.is_empty() {
            let unsigned_tx = ProtobufBuilder::new(&input).build_tx()?;
            Self::sign_tx(unsigned_tx, input)
        } else {
            let json_tx = ProtobufBuilder::new(&input).build_tx_json()?;
            Self::sign_tx(json_tx, input)
        }
    }

    fn sign_tx<Transaction: RippleTransaction>(
        unsigned_tx: Transaction,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let private_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())
            .into_tw()
            .context("Invalid private key")?;
        let signed_tx = TransactionSigner::sign(unsigned_tx, &private_key)?;

        let signing_only = false;
        let encoded = encode_tx(&signed_tx, signing_only)?.encoded;
        Ok(Proto::SigningOutput {
            encoded: encoded.into(),
            ..Proto::SigningOutput::default()
        })
    }
}
