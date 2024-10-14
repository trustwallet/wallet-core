// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::compiler::TheOpenNetworkCompiler;
use crate::modules::transaction_util::TonTransactionUtil;
use crate::signer::TheOpenNetworkSigner;
use crate::wallet::{wallet_v4, VersionedTonWallet};
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
use tw_proto::TheOpenNetwork::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct TheOpenNetworkEntry;

impl CoinEntry for TheOpenNetworkEntry {
    type AddressPrefix = NoPrefix;
    type Address = TonAddress;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;
    type TransactionUtil = TonTransactionUtil;

    #[inline]
    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        // TODO consider checking whether the transaction is on testnet.
        TonAddress::from_str(address).and_then(TonAddress::normalize)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        TonAddress::from_str(address).and_then(TonAddress::normalize)
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let ed25519_pubkey = public_key
            .to_ed25519()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;
        // Currently, we use the V4R2 wallet
        let wallet = VersionedTonWallet::V4R2(
            wallet_v4::WalletV4R2::std_with_public_key(ed25519_pubkey.clone())
                .map_err(|_| AddressError::Internal)?,
        );
        Ok(wallet.address().clone())
    }

    #[inline]
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        TheOpenNetworkSigner::sign(coin, input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        TheOpenNetworkCompiler::preimage_hashes(coin, input)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        TheOpenNetworkCompiler::compile(coin, input, signatures, public_keys)
    }

    #[inline]
    fn transaction_util(&self) -> Option<Self::TransactionUtil> {
        Some(TonTransactionUtil)
    }
}
