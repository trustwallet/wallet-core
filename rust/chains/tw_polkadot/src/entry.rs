// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ctx_from_tw;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ed25519::sha512::{KeyPair, PublicKey};
use tw_keypair::traits::KeyPairTrait;
use tw_number::U256;
use tw_proto::Polkadot::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_scale::ToScale;
use tw_ss58_address::SS58Address;
use tw_ss58_address::NetworkId;
use tw_substrate::*;

use crate::call_encoder::CallEncoder;

fn require_check_metadata(network_id: NetworkId, spec_version: u32) -> bool {
    match network_id {
        NetworkId::POLKADOT | NetworkId::KUSAMA if spec_version >= 1_002_005 => true,
        _ => false,
    }
}

pub struct PolkadotEntry;

impl PolkadotEntry {
    #[inline]
    fn build_transaction_impl(
        &self,
        _coin: &dyn CoinContext,
        mut public_key: Option<PublicKey>,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<TransactionBuilder> {
        let ctx = ctx_from_tw(&input)?;
        let encoder = CallEncoder::from_ctx(&ctx)?;
        let call = encoder.encode_call(&input.message_oneof)?;
        let check_metadata = require_check_metadata(ctx.network, input.spec_version);
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

        let mut builder = TransactionBuilder::new(ctx.multi_address, call);
        // Add chain extensions.
        builder.extension(CheckVersion(input.spec_version));
        builder.extension(CheckVersion(input.transaction_version));
        builder.extension(CheckGenesis(genesis_hash));
        builder.extension(CheckEra { era, current_hash });
        builder.extension(CheckNonce::new(input.nonce as u32));
        if let Some(fee_asset_id) = ctx.fee_asset_id {
            builder.extension(ChargeAssetTxPayment::new(tip, fee_asset_id));
        } else {
            builder.extension(ChargeTransactionPayment::new(tip));
        }
        if check_metadata {
            builder.extension(CheckMetadataHash::default());
        }
        if input.private_key.len() > 0 {
            if let Ok(keypair) = KeyPair::try_from(input.private_key.as_ref()) {
                public_key = Some(keypair.public().clone());
                builder.set_keypair(keypair);
            }
        }
        if let Some(public_key) = public_key {
            let account = SubstrateAddress(SS58Address::from_public_key(&public_key, ctx.network)?);
            builder.set_account(account);
        }
        Ok(builder)
    }

    #[inline]
    fn signing_output_impl(
        &self,
        _coin: &dyn CoinContext,
        result: SigningResult<Encoded>,
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
        result: SigningResult<Encoded>,
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

impl SubstrateCoinEntry for PolkadotEntry {
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    #[inline]
    fn build_transaction(
        &self,
        coin: &dyn CoinContext,
        public_key: Option<PublicKey>,
        input: &Self::SigningInput<'_>,
    ) -> SigningResult<TransactionBuilder> {
        self.build_transaction_impl(coin, public_key, input)
    }

    #[inline]
    fn signing_output(
        &self,
        coin: &dyn CoinContext,
        result: SigningResult<Encoded>,
    ) -> Self::SigningOutput {
        self.signing_output_impl(coin, result)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    #[inline]
    fn presigning_output(
        &self,
        coin: &dyn CoinContext,
        result: SigningResult<Encoded>,
    ) -> Self::PreSigningOutput {
        self.presigning_output_impl(coin, result)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }
}
