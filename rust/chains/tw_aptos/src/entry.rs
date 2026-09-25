// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::compiler::Compiler;
use crate::modules::transaction_util::AptosTransactionUtil;
use crate::signer::Signer;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::PublicKey;
use tw_proto::Aptos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct AptosEntry;

impl CoinEntry for AptosEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;
    type TransactionUtil = AptosTransactionUtil;

    #[inline]
    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        Address::from_str(address)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        Address::from_str(address)
    }

    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let public_key = public_key
            .to_ed25519()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;
        Address::with_ed25519_pubkey(public_key)
    }

    #[inline]
    fn sign(&self, _coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        Signer::sign_proto(input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        Compiler::preimage_hashes(input)
    }

    #[inline]
    fn compile(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        Compiler::compile(input, signatures, public_keys)
    }

    #[inline]
    fn json_signer(&self) -> Option<Self::JsonSigner> {
        None
    }

    #[inline]
    fn message_signer(&self) -> Option<Self::MessageSigner> {
        None
    }

    #[inline]
    fn transaction_util(&self) -> Option<Self::TransactionUtil> {
        Some(AptosTransactionUtil)
    }
}
