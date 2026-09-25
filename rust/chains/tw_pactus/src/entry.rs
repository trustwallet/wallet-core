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
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::PublicKey;
use tw_proto::Pactus::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

use crate::compiler::PactusCompiler;
use crate::modules::transaction_util::PactusTransactionUtil;
use crate::signer::PactusSigner;
use crate::types::network::Network;
use crate::types::Address;

pub struct PactusEntry;

impl CoinEntry for PactusEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;
    type TransactionUtil = PactusTransactionUtil;

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
        Address::from_str(address)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        Address::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let public_key = public_key
            .to_ed25519()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;

        match derivation {
            Derivation::Default => Address::from_public_key(public_key, Network::Mainnet),
            Derivation::Testnet => Address::from_public_key(public_key, Network::Testnet),
            _ => AddressResult::Err(AddressError::Unsupported),
        }
    }

    #[inline]
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        PactusSigner::sign(coin, input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        PactusCompiler::preimage_hashes(coin, input)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        PactusCompiler::compile(coin, input, signatures, public_keys)
    }

    #[inline]
    fn transaction_util(&self) -> Option<Self::TransactionUtil> {
        Some(PactusTransactionUtil)
    }
}
