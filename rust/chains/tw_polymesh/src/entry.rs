// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ctx_from_tw;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ed25519::sha512::{KeyPair, PublicKey};
use tw_number::U256;
use tw_proto::Polymesh::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_scale::{RawOwned, ToScale};
use tw_ss58_address::SS58Address;
use tw_substrate::*;

use crate::call_encoder::CallEncoder;

pub struct PolymeshEntry;

impl PolymeshEntry {
    #[inline]
    fn get_keypair_impl(
        &self,
        _coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<KeyPair> {
        Ok(KeyPair::try_from(input.private_key.as_ref())?)
    }

    #[inline]
    fn build_transaction_impl(
        &self,
        _coin: &dyn CoinContext,
        public_key: Option<PublicKey>,
        input: &Proto::SigningInput<'_>,
    ) -> EncodeResult<TransactionBuilder> {
        let ctx = ctx_from_tw(input)?;
        let mut encoder = CallEncoder::from_ctx(&ctx);
        let call = input
            .runtime_call
            .as_ref()
            .ok_or(EncodeError::InvalidValue)
            .into_tw()
            .context("Missing runtime call")?;
        let call = encoder.encode_runtime_call(call)?;
        let era = match &input.era {
            Some(era) => Era::mortal(era.period, era.block_number),
            None => Era::immortal(),
        };
        let genesis_hash = input.genesis_hash.as_ref().try_into().unwrap_or_default();
        let current_hash = input.block_hash.as_ref().try_into().unwrap_or_default();
        let tip = U256::from_big_endian_slice(&input.tip)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        let mut builder = TransactionBuilder::new(true, call);
        // Add chain extensions.
        builder.extension(CheckVersion(input.spec_version));
        builder.extension(CheckVersion(input.transaction_version));
        builder.extension(CheckGenesis(genesis_hash));
        builder.extension(CheckEra { era, current_hash });
        builder.extension(CheckNonce::new(input.nonce as u32));
        builder.extension(ChargeTransactionPayment::new(tip));
        if let Some(public_key) = public_key {
            let account = SubstrateAddress(
                SS58Address::from_public_key(&public_key, ctx.network).map_err(|e| {
                    TWError::new(EncodeError::InvalidAddress).context(format!("{e:?}"))
                })?,
            );
            builder.set_account(account);
        }
        Ok(builder)
    }

    #[inline]
    fn signing_output_impl(
        &self,
        _coin: &dyn CoinContext,
        result: SigningResult<RawOwned>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let encoded = result?.to_scale();
        Ok(Proto::SigningOutput {
            encoded: encoded.into(),
            ..Default::default()
        })
    }

    #[inline]
    fn presigning_output_impl(
        &self,
        _coin: &dyn CoinContext,
        result: SigningResult<RawOwned>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let pre_image = result?.to_scale();
        Ok(CompilerProto::PreSigningOutput {
            // `pre_image` is already hashed if it is larger then 256 bytes.
            data_hash: pre_image.clone().into(),
            data: pre_image.into(),
            ..Default::default()
        })
    }
}

impl SubstrateCoinEntry for PolymeshEntry {
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    #[inline]
    fn get_keypair(
        &self,
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<KeyPair> {
        self.get_keypair_impl(coin, input)
    }

    #[inline]
    fn build_transaction(
        &self,
        coin: &dyn CoinContext,
        public_key: Option<PublicKey>,
        input: &Self::SigningInput<'_>,
    ) -> SigningResult<TransactionBuilder> {
        self.build_transaction_impl(coin, public_key, input)
            .map_err(|e| e.map_err(SigningErrorType::from))
    }

    #[inline]
    fn signing_output(
        &self,
        coin: &dyn CoinContext,
        result: SigningResult<RawOwned>,
    ) -> Self::SigningOutput {
        self.signing_output_impl(coin, result)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    #[inline]
    fn presigning_output(
        &self,
        coin: &dyn CoinContext,
        result: SigningResult<RawOwned>,
    ) -> Self::PreSigningOutput {
        self.presigning_output_impl(coin, result)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }
}
