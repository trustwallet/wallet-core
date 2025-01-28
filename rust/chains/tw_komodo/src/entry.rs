// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::KomodoContext;
use std::str::FromStr;
use tw_bitcoin::modules::compiler::BitcoinCompiler;
use tw_bitcoin::modules::planner::BitcoinPlanner;
use tw_bitcoin::modules::signer::BitcoinSigner;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::transaction_util::NoTransactionUtil;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_coin_entry::prefix::BitcoinBase58Prefix;
use tw_keypair::tw::PublicKey;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::address::legacy::LegacyAddress;

pub struct KomodoEntry;

impl CoinEntry for KomodoEntry {
    type AddressPrefix = BitcoinBase58Prefix;
    type Address = LegacyAddress;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = Proto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = BitcoinPlanner<KomodoContext>;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;
    type TransactionUtil = NoTransactionUtil;

    #[inline]
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        LegacyAddress::from_str_with_coin_and_prefix(coin, address, prefix)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        LegacyAddress::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        LegacyAddress::p2pkh_with_coin_and_prefix(coin, &public_key, prefix)
    }

    #[inline]
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        BitcoinSigner::<KomodoContext>::sign(coin, &input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        BitcoinCompiler::<KomodoContext>::preimage_hashes(coin, input)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        BitcoinCompiler::<KomodoContext>::compile(coin, input, signatures, public_keys)
    }

    #[inline]
    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        Some(BitcoinPlanner::<KomodoContext>::default())
    }
}
