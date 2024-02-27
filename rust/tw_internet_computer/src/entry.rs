// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;

use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::{
    coin_context::CoinContext,
    coin_entry::CoinEntry,
    error::{AddressError, AddressResult, SigningError},
    modules::{
        json_signer::NoJsonSigner, message_signer::NoMessageSigner, plan_builder::NoPlanBuilder,
        wallet_connector::NoWalletConnector,
    },
    prefix::NoPrefix,
    signing_output_error,
};

use tw_proto::{
    Common::Proto::SigningError as CommonError, InternetComputer::Proto,
    TxCompiler::Proto as CompilerProto,
};

use crate::{address::AccountIdentifier, context::StandardInternetComputerContext, signer::Signer};

pub struct InternetComputerEntry;

impl CoinEntry for InternetComputerEntry {
    type AddressPrefix = NoPrefix;
    type Address = AccountIdentifier;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;

    #[inline]
    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        Self::Address::from_str(address)
    }

    #[inline]
    fn parse_address_unchecked(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
    ) -> AddressResult<Self::Address> {
        Self::Address::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn tw_coin_entry::coin_context::CoinContext,
        public_key: tw_keypair::tw::PublicKey,
        _derivation: tw_coin_entry::derivation::Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> tw_coin_entry::error::AddressResult<Self::Address> {
        let secp256k1_public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;
        Ok(Self::Address::from(secp256k1_public_key))
    }

    #[inline]
    fn sign(
        &self,
        _coin: &dyn tw_coin_entry::coin_context::CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::SigningOutput {
        Signer::<StandardInternetComputerContext>::sign_proto(input)
    }

    fn preimage_hashes(
        &self,
        _coin: &dyn tw_coin_entry::coin_context::CoinContext,
        _input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        signing_output_error!(
            CompilerProto::PreSigningOutput,
            SigningError(CommonError::Error_not_supported)
        )
    }

    fn compile(
        &self,
        _coin: &dyn tw_coin_entry::coin_context::CoinContext,
        _input: Self::SigningInput<'_>,
        _signatures: Vec<tw_coin_entry::coin_entry::SignatureBytes>,
        _public_keys: Vec<tw_coin_entry::coin_entry::PublicKeyBytes>,
    ) -> Self::SigningOutput {
        signing_output_error!(
            Proto::SigningOutput,
            SigningError(CommonError::Error_not_supported)
        )
    }
}
