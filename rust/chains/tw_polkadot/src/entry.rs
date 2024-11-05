// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::compiler::PolkadotCompiler;
use crate::signer::PolkadotSigner;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::transaction_util::NoTransactionUtil;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_keypair::tw::PublicKey;
use tw_proto::Polkadot::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_ss58_address::{NetworkId, SS58Address};
use tw_substrate::address::{SubstrateAddress, SubstratePrefix};

pub struct PolkadotEntry;

impl CoinEntry for PolkadotEntry {
    type AddressPrefix = SubstratePrefix;
    type Address = SubstrateAddress;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;
    type TransactionUtil = NoTransactionUtil;

    #[inline]
    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        SubstrateAddress::from_str(address)?.with_network_check(prefix)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        SubstrateAddress::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let public_key = public_key
            .to_ed25519()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;

        SS58Address::from_public_key(
            public_key,
            prefix
                .map(SubstratePrefix::network)
                .unwrap_or(NetworkId::POLKADOT),
        )
        .map(SubstrateAddress)
    }

    #[inline]
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        PolkadotSigner::sign(coin, input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        PolkadotCompiler::preimage_hashes(coin, input)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        PolkadotCompiler::compile(coin, input, signatures, public_keys)
    }
}
