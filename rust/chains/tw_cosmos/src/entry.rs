// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
use tw_cosmos_sdk::address::{Address, Bech32Prefix};
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::modules::compiler::tw_compiler::TWTransactionCompiler;
use tw_cosmos_sdk::modules::signer::tw_signer::TWSigner;
use tw_cosmos_sdk::modules::transaction_util::CosmosTransactionUtil;
use tw_keypair::tw;
use tw_proto::Cosmos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct CosmosEntry;

impl CoinEntry for CosmosEntry {
    type AddressPrefix = Bech32Prefix;
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
    type TransactionUtil = CosmosTransactionUtil<StandardCosmosContext>;

    #[inline]
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        Address::from_str_with_coin_and_prefix(coin, address.to_string(), prefix)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        Address::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: tw::PublicKey,
        _derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        Address::with_public_key_coin_context(coin, &public_key, prefix)
    }

    #[inline]
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        TWSigner::<StandardCosmosContext>::sign(coin, input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        TWTransactionCompiler::<StandardCosmosContext>::preimage_hashes(coin, input)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        TWTransactionCompiler::<StandardCosmosContext>::compile(
            coin,
            input,
            signatures,
            public_keys,
        )
    }

    #[inline]
    fn transaction_util(&self) -> Option<Self::TransactionUtil> {
        Some(CosmosTransactionUtil::<StandardCosmosContext>::default())
    }
}
