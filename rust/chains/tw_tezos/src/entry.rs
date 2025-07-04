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
use tw_coin_entry::modules::transaction_util::NoTransactionUtil;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::PublicKey;
use tw_proto::Tezos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

use crate::address::TezosAddress;
use crate::compiler::TezosCompiler;
use crate::signer::TezosSigner;

pub struct TezosEntry;

impl CoinEntry for TezosEntry {
    type AddressPrefix = NoPrefix;
    type Address = TezosAddress;
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
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        TezosAddress::from_str(address)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        TezosAddress::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        TezosAddress::with_public_key(&public_key)
    }

    #[inline]
    fn sign(&self, _coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        TezosSigner::sign(_coin, input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        TezosCompiler::preimage_hashes(_coin, input)
    }

    #[inline]
    fn compile(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        TezosCompiler::compile(_coin, input, signatures, public_keys)
    }

    #[inline]
    fn message_signer(&self) -> Option<Self::MessageSigner> {
        None
    }

    #[inline]
    fn transaction_util(&self) -> Option<Self::TransactionUtil> {
        None
    }
}
