// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::BinanceAddress;
use crate::compiler::BinanceCompiler;
use crate::modules::wallet_connect::connector::BinanceWalletConnector;
use crate::signer::BinanceSigner;
use std::str::FromStr;
use tw_bech32_address::bech32_prefix::Bech32Prefix;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::transaction_util::NoTransactionUtil;
use tw_keypair::tw::PublicKey;
use tw_proto::Binance::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct BinanceEntry;

impl CoinEntry for BinanceEntry {
    type AddressPrefix = Bech32Prefix;
    type Address = BinanceAddress;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = BinanceWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;
    type TransactionUtil = NoTransactionUtil;

    #[inline]
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        BinanceAddress::from_str_with_coin_and_prefix(coin, address.to_string(), prefix)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        BinanceAddress::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        BinanceAddress::with_public_key_coin_context(coin, &public_key, prefix)
    }

    #[inline]
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        BinanceSigner::sign(coin, input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        BinanceCompiler::preimage_hashes(coin, input)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        BinanceCompiler::compile(coin, input, signatures, public_keys)
    }

    #[inline]
    fn wallet_connector(&self) -> Option<Self::WalletConnector> {
        Some(BinanceWalletConnector)
    }
}
