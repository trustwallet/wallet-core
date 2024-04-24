// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_coin_entry::prefix::NoPrefix;
use tw_evm::address::Address;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::evm_entry::EvmEntry;
use tw_evm::modules::compiler::Compiler;
use tw_evm::modules::json_signer::EthJsonSigner;
use tw_evm::modules::message_signer::EthMessageSigner;
use tw_evm::modules::signer::Signer;
use tw_keypair::tw::PublicKey;
use tw_proto::Ethereum::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct EthereumEntry;

impl CoinEntry for EthereumEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = EthJsonSigner<StandardEvmContext>;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = EthMessageSigner;
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
    fn parse_address_unchecked(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
    ) -> AddressResult<Self::Address> {
        Address::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;
        Ok(Address::with_secp256k1_pubkey(public_key))
    }

    #[inline]
    fn sign(&self, _coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        Signer::<StandardEvmContext>::sign_proto(input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        Compiler::<StandardEvmContext>::preimage_hashes(input)
    }

    #[inline]
    fn compile(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        Compiler::<StandardEvmContext>::compile(input, signatures, public_keys)
    }

    #[inline]
    fn json_signer(&self) -> Option<Self::JsonSigner> {
        Some(EthJsonSigner::default())
    }

    #[inline]
    fn message_signer(&self) -> Option<Self::MessageSigner> {
        Some(EthMessageSigner)
    }
}

impl EvmEntry for EthereumEntry {
    type Context = StandardEvmContext;
}
